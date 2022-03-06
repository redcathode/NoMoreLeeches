#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem> // or #include <filesystem> for C++17 and up
#include <algorithm>

    
namespace fs = std::experimental::filesystem;


#include "main_window.h"
#include "blocked_window.h"
#define WINDOW_CHECK_TIMES_PER_SECOND 0.5

#ifdef _WIN32 // TODO: actual windows support
#define DIRSEP "\\"
#else
#define DIRSEP "/"
#include "x11_window_manager.h"
x11_window_manager *windowManager = new x11_window_manager();
#endif

Fl_Text_Buffer wildcardsBuf;
Fl_Text_Buffer windNamesBuf;
std::string configPath;
main_window *mainWindow = new main_window();
bool enabled = false;
bool currentlyBlocking = false;
blocked_window *blockedWindow = nullptr;
int defaultSecondsLeft = 15;
int currentSecondsLeft = 15;
bool timerIsStopped = true;
std::string currentOkClass = "hopefully there isn't a window matching this text exactly";
int violations = 0;

bool str_replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}
void minimizeWindowTimeout(void*) {
    blockedWindow->BlockWindow->show();
    blockedWindow->BlockWindow->resize(windowManager->focusedWindowX, windowManager->focusedWindowY, windowManager->focusedWindowWidth, windowManager->focusedWindowHeight);
    windowManager->toggle_window_state(false);
}
void showBrieflyCallback(Fl_Widget*) {
    windowManager->toggle_window_state(false);
    blockedWindow->BlockWindow->hide();
    currentSecondsLeft = 60;
    Fl::add_timeout(5.0, minimizeWindowTimeout);
}
void blocklistSavedCallback(void*) {
  mainWindow->SaveBlocklistButton->label("Save Blocklists");
}
void tickWindowShowTimer(void*) {
  if (blockedWindow == nullptr) {
    return;
  }
  currentSecondsLeft -= 1;
  if (!windowManager->is_currently_active_window_owned_by_us()) {
    currentSecondsLeft = 60;
    std::cout << "resetting timer";
  }
  //blockedWindow->WaitDisplay->label();
  std::string timerText;
  timerText = "If you really want to access this window,\nwait secks:flushed: seconds.";
  std::string timerNum = std::to_string(currentSecondsLeft);
  str_replace(timerText, "secks:flushed:", timerNum);
  char *timerChar = new char[timerText.size() + 1];
  std::copy(timerText.begin(), timerText.end(), timerChar);
  blockedWindow->WaitDisplay->label(timerChar);
  if (currentSecondsLeft == 0) {
    currentOkClass = windowManager->active_window_class;
    blockedWindow->BlockWindow->hide();
    delete blockedWindow;
    blockedWindow = nullptr;
    windowManager->toggle_window_state(true);
  } else {
    Fl::repeat_timeout(1.0, tickWindowShowTimer);
  }
}
void save_blocklists(Fl_Widget*) {
  std::ofstream wildcardsFile;
  std::ofstream windowNamesFile;
  wildcardsFile.open(configPath + DIRSEP + "wildcardsList.txt", std::fstream::out | std::fstream::trunc);
  windowNamesFile.open(configPath + DIRSEP + "windowNamesList.txt", std::fstream::out | std::fstream::trunc);
  wildcardsFile << wildcardsBuf.text();
  windowNamesFile << windNamesBuf.text();
  mainWindow->SaveBlocklistButton->label("Saved.");
  Fl::add_timeout(0.7, blocklistSavedCallback);
}
void toggleNMLEnable(Fl_Widget*) {
  enabled = !enabled;
  if (enabled) {
    mainWindow->ToggleNMLButton->label("Enable NML\n(currently disabled)");
  } else {
    mainWindow->ToggleNMLButton->label("Disable NML\n(currently enabled)");
  }
}

std::string getConfigDir() {
  std::string path;
  if (const char* env_p = std::getenv("APPDATA")) {
        std::cout << "Config dir: " << env_p << '\n';
        path.append(env_p);
        path.append("\\nomoreleeches\\");
  } else if (const char* env_p = std::getenv("XDG_CONFIG_HOME")) {
        path.append(env_p);
        path.append("/nomoreleeches/");
  } else if (const char* env_p = std::getenv("HOME")) {
        path.append(env_p);
        path.append("/.config/nomoreleeches/");
  } else {
        // ????????
        exit(69);
  }
  if (!fs::is_directory(path) || !fs::exists(path)) { // Check if src folder exists
    fs::create_directory(path); // create src folder
  }
  std::cout << "Config dir: " << path << std::endl;
  return path;
}
void doThingWithWindows(void*) {
  std::cout << currentOkClass << std::endl;
  if (blockedWindow == nullptr && windowManager->update()) {
    if (windowManager->active_window_class == currentOkClass || windowManager->is_currently_active_window_owned_by_us()) {
      std::cout << "active window class: " << windowManager->active_window_class << std::endl;
      Fl::repeat_timeout(WINDOW_CHECK_TIMES_PER_SECOND, doThingWithWindows);
      return;
    } else {
      currentOkClass = "hopefully there isn't a window matching this text exactly";
    }
    // std::cout << windowManager->active_window_class << std::endl;
    if (windowManager->active_window_class != "FLTK") { // todo: this will also whitelist other fltk apps. unimportant, since no apps that people actually use are written in fltk
      mainWindow->CurrentWindowClassOutput->value(windowManager->active_window_class.c_str());
      mainWindow->CurrentWindowNameOutput->value(windowManager->active_window_name.c_str());
    }
   } //else { // commenting this out as we should just keep the last window that was grabbed successfully
  //   mainWindow->CurrentWindowClassOutput->value("(error getting window name)");
  //   mainWindow->CurrentWindowNameOutput->value("(error getting window name)");
  // }
  std::vector<std::string> classBlockList = split_string(windNamesBuf.text(), "\n");
  if (std::count(classBlockList.begin(), classBlockList.end(), windowManager->active_window_class)) {
    //std::cout << "h" << std::endl;
    if (blockedWindow == nullptr) {
      currentlyBlocking = true;
      blockedWindow = new blocked_window();
      blockedWindow->make_window();
      blockedWindow->BlockWindow->resize(windowManager->focusedWindowX, windowManager->focusedWindowY, windowManager->focusedWindowWidth, windowManager->focusedWindowHeight);
      std::cout << windowManager->focusedWindowX << ":" << windowManager->focusedWindowY << ":" << windowManager->focusedWindowWidth << ":"  << windowManager->focusedWindowHeight <<  std::endl;
      std::string reasonText = "Reason: 'windowclass' matched item in blocked window class list";
      std::string windowText = "Window 'windowname' blocked.";
      std::string applicationName = windowManager->active_window_name;
      std::string applicationGame = windowManager->active_window_class;
      str_replace(reasonText, "windowclass", applicationGame);
      str_replace(windowText, "windowname", applicationName);
      char *reasonChar = new char[reasonText.size() + 1];
      std::copy(reasonText.begin(), reasonText.end(), reasonChar);

      char *windowChar = new char[windowText.size() + 1];
      std::copy(windowText.begin(), windowText.end(), windowChar);
      blockedWindow->ReasonDisplay->label(reasonChar);
      blockedWindow->WindowHiddenDisplay->label(windowChar);
      //blockedWindow->make_window(0, 0, 250, 250);
      //std::cout << windowManager->focusedWindowX << std::endl;
      windowManager->toggle_window_state(true);
      //blockedWindow->BlockWindow->border(0);
      // blockedWindow->BlockWindow->fullscreen();
      blockedWindow->BlockWindow->show();
      blockedWindow->ShowBrieflyButton->callback(showBrieflyCallback);
      Fl::add_timeout(1.0, tickWindowShowTimer);
      

      // Fl::grab(blockedWindow->BlockWindow);
    }
  }
  Fl::repeat_timeout(WINDOW_CHECK_TIMES_PER_SECOND, doThingWithWindows);
}

void setColor(int one, int two, int three, int four) {
  Fl::set_color(Fl_Color(one), two, three, four);
}
void loadColorScheme() {
    setColor(0, 140, 140, 140);
    setColor(1, 120, 0, 0);
    setColor(2, 0, 120, 0);
    setColor(3, 120, 120, 0);
    setColor(4, 0, 0, 120);
    setColor(5, 120, 0, 120);
    setColor(6, 0, 120, 120);
    setColor(7, 0, 0, 0);
    setColor(8, 100, 100, 100);
    setColor(9, 100, 0, 0);
    setColor(10, 0, 100, 0);
    setColor(11, 100, 100, 0);
    setColor(12, 0, 0, 100);
    setColor(13, 100, 0, 100);
    setColor(14, 0, 100, 100);
    setColor(15, 50, 50, 50);
    setColor(32, 180, 180, 180);
    setColor(33, 122, 122, 122);
    setColor(34, 117, 117, 117);
    setColor(35, 112, 112, 112);
    setColor(36, 106, 106, 106);
    setColor(37, 101, 101, 101);
    setColor(38, 96, 96, 96);
    setColor(39, 90, 90, 90);
    setColor(40, 85, 85, 85);
    setColor(41, 80, 80, 80);
    setColor(42, 74, 74, 74);
    setColor(43, 69, 69, 69);
    setColor(44, 64, 64, 64);
    setColor(45, 58, 58, 58);
    setColor(46, 53, 53, 53);
    setColor(47, 48, 48, 48);
    setColor(48, 42, 42, 42);
    setColor(49, 37, 37, 37);
    setColor(50, 32, 32, 32);
    setColor(51, 26, 26, 26);
    setColor(52, 21, 21, 21);
    setColor(53, 16, 16, 16);
    setColor(54, 10, 10, 10);
    setColor(55, 0, 0, 0);
    setColor(56, 0, 0, 0);
    setColor(57, 0, 16, 0);
    setColor(58, 0, 31, 0);
    setColor(59, 0, 47, 0);
    setColor(60, 0, 63, 0);
    setColor(61, 0, 79, 0);
    setColor(62, 0, 94, 0);
    setColor(63, 0, 110, 0);
    setColor(64, 28, 0, 0);
    setColor(65, 28, 16, 0);
    setColor(66, 28, 31, 0);
    setColor(67, 28, 47, 0);
    setColor(68, 28, 63, 0);
    setColor(69, 28, 79, 0);
    setColor(70, 28, 94, 0);
    setColor(71, 28, 110, 0);
    setColor(72, 55, 0, 0);
    setColor(73, 55, 16, 0);
    setColor(74, 55, 31, 0);
    setColor(75, 55, 47, 0);
    setColor(76, 55, 63, 0);
    setColor(77, 55, 79, 0);
    setColor(78, 55, 94, 0);
    setColor(79, 55, 110, 0);
    setColor(80, 83, 0, 0);
    setColor(81, 83, 16, 0);
    setColor(82, 83, 31, 0);
    setColor(83, 83, 47, 0);
    setColor(84, 83, 63, 0);
    setColor(85, 83, 79, 0);
    setColor(86, 83, 94, 0);
    setColor(87, 83, 110, 0);
    setColor(88, 110, 0, 0);
    setColor(89, 110, 16, 0);
    setColor(90, 110, 31, 0);
    setColor(91, 110, 47, 0);
    setColor(92, 110, 63, 0);
    setColor(93, 110, 79, 0);
    setColor(94, 110, 94, 0);
    setColor(95, 110, 110, 0);
    setColor(96, 0, 0, 28);
    setColor(97, 0, 16, 28);
    setColor(98, 0, 31, 28);
    setColor(99, 0, 47, 28);
    setColor(100, 0, 63, 28);
    setColor(101, 0, 79, 28);
    setColor(102, 0, 94, 28);
    setColor(103, 0, 110, 28);
    setColor(104, 28, 0, 28);
    setColor(105, 28, 16, 28);
    setColor(106, 28, 31, 28);
    setColor(107, 28, 47, 28);
    setColor(108, 28, 63, 28);
    setColor(109, 28, 79, 28);
    setColor(110, 28, 94, 28);
    setColor(111, 28, 110, 28);
    setColor(112, 55, 0, 28);
    setColor(113, 55, 16, 28);
    setColor(114, 55, 31, 28);
    setColor(115, 55, 47, 28);
    setColor(116, 55, 63, 28);
    setColor(117, 55, 79, 28);
    setColor(118, 55, 94, 28);
    setColor(119, 55, 110, 28);
    setColor(120, 83, 0, 28);
    setColor(121, 83, 16, 28);
    setColor(122, 83, 31, 28);
    setColor(123, 83, 47, 28);
    setColor(124, 83, 63, 28);
    setColor(125, 83, 79, 28);
    setColor(126, 83, 94, 28);
    setColor(127, 83, 110, 28);
    setColor(128, 110, 0, 28);
    setColor(129, 110, 16, 28);
    setColor(130, 110, 31, 28);
    setColor(131, 110, 47, 28);
    setColor(132, 110, 63, 28);
    setColor(133, 110, 79, 28);
    setColor(134, 110, 94, 28);
    setColor(135, 110, 110, 28);
    setColor(136, 0, 0, 55);
    setColor(137, 0, 16, 55);
    setColor(138, 0, 31, 55);
    setColor(139, 0, 47, 55);
    setColor(140, 0, 63, 55);
    setColor(141, 0, 79, 55);
    setColor(142, 0, 94, 55);
    setColor(143, 0, 110, 55);
    setColor(144, 28, 0, 55);
    setColor(145, 28, 16, 55);
    setColor(146, 28, 31, 55);
    setColor(147, 28, 47, 55);
    setColor(148, 28, 63, 55);
    setColor(149, 28, 79, 55);
    setColor(150, 28, 94, 55);
    setColor(151, 28, 110, 55);
    setColor(152, 55, 0, 55);
    setColor(153, 55, 16, 55);
    setColor(154, 55, 31, 55);
    setColor(155, 55, 47, 55);
    setColor(156, 55, 63, 55);
    setColor(157, 55, 79, 55);
    setColor(158, 55, 94, 55);
    setColor(159, 55, 110, 55);
    setColor(160, 83, 0, 55);
    setColor(161, 83, 16, 55);
    setColor(162, 83, 31, 55);
    setColor(163, 83, 47, 55);
    setColor(164, 83, 63, 55);
    setColor(165, 83, 79, 55);
    setColor(166, 83, 94, 55);
    setColor(167, 83, 110, 55);
    setColor(168, 110, 0, 55);
    setColor(169, 110, 16, 55);
    setColor(170, 110, 31, 55);
    setColor(171, 110, 47, 55);
    setColor(172, 110, 63, 55);
    setColor(173, 110, 79, 55);
    setColor(174, 110, 94, 55);
    setColor(175, 110, 110, 55);
    setColor(176, 0, 0, 83);
    setColor(177, 0, 16, 83);
    setColor(178, 0, 31, 83);
    setColor(179, 0, 47, 83);
    setColor(180, 0, 63, 83);
    setColor(181, 0, 79, 83);
    setColor(182, 0, 94, 83);
    setColor(183, 0, 110, 83);
    setColor(184, 28, 0, 83);
    setColor(185, 28, 16, 83);
    setColor(186, 28, 31, 83);
    setColor(187, 28, 47, 83);
    setColor(188, 28, 63, 83);
    setColor(189, 28, 79, 83);
    setColor(190, 28, 94, 83);
    setColor(191, 28, 110, 83);
    setColor(192, 55, 0, 83);
    setColor(193, 55, 16, 83);
    setColor(194, 55, 31, 83);
    setColor(195, 55, 47, 83);
    setColor(196, 55, 63, 83);
    setColor(197, 55, 79, 83);
    setColor(198, 55, 94, 83);
    setColor(199, 55, 110, 83);
    setColor(200, 83, 0, 83);
    setColor(201, 83, 16, 83);
    setColor(202, 83, 31, 83);
    setColor(203, 83, 47, 83);
    setColor(204, 83, 63, 83);
    setColor(205, 83, 79, 83);
    setColor(206, 83, 94, 83);
    setColor(207, 83, 110, 83);
    setColor(208, 110, 0, 83);
    setColor(209, 110, 16, 83);
    setColor(210, 110, 31, 83);
    setColor(211, 110, 47, 83);
    setColor(212, 110, 63, 83);
    setColor(213, 110, 79, 83);
    setColor(214, 110, 94, 83);
    setColor(215, 110, 110, 83);
    setColor(216, 0, 0, 110);
    setColor(217, 0, 16, 110);
    setColor(218, 0, 31, 110);
    setColor(219, 0, 47, 110);
    setColor(220, 0, 63, 110);
    setColor(221, 0, 79, 110);
    setColor(222, 0, 94, 110);
    setColor(223, 0, 110, 110);
    setColor(224, 28, 0, 110);
    setColor(225, 28, 16, 110);
    setColor(226, 28, 31, 110);
    setColor(227, 28, 47, 110);
    setColor(228, 28, 63, 110);
    setColor(229, 28, 79, 110);
    setColor(230, 28, 94, 110);
    setColor(231, 28, 110, 110);
    setColor(232, 55, 0, 110);
    setColor(233, 55, 16, 110);
    setColor(234, 55, 31, 110);
    setColor(235, 55, 47, 110);
    setColor(236, 55, 63, 110);
    setColor(237, 55, 79, 110);
    setColor(238, 55, 94, 110);
    setColor(239, 55, 110, 110);
    setColor(240, 83, 0, 110);
    setColor(241, 83, 16, 110);
    setColor(242, 83, 31, 110);
    setColor(243, 83, 47, 110);
    setColor(244, 83, 63, 110);
    setColor(245, 83, 79, 110);
    setColor(246, 83, 94, 110);
    setColor(247, 83, 110, 110);
    setColor(248, 110, 0, 110);
    setColor(249, 110, 16, 110);
    setColor(250, 110, 31, 110);
    setColor(251, 110, 47, 110);
    setColor(252, 110, 63, 110);
    setColor(253, 110, 79, 110);
    setColor(254, 110, 94, 110);
    setColor(255, 110, 110, 110);
    setColor(0, 140, 140, 140);
    setColor(1, 180, 40, 40);
    setColor(2, 0, 120, 0);
    setColor(4, 100, 100, 240);
    setColor(5, 160, 60, 160);
    setColor(9, 100, 0, 0);
    setColor(13, 60, 20, 60);
    setColor(15, 200, 50, 50);
    setColor(59, 40, 140, 40);
    setColor(90, 160, 120, 40);
    setColor(94, 120, 80, 20);
    setColor(95, 140, 140, 100);
    setColor(124, 90, 68, 30);
    setColor(254, 50, 70, 70);
    setColor(255, 70, 70, 70);
}
int main(int argc, char **argv) {
  configPath = getConfigDir();
  std::ofstream wildcardsFile;
  std::ofstream windowNamesFile;
  wildcardsFile.open(configPath + DIRSEP + "wildcardsList.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  windowNamesFile.open(configPath + DIRSEP + "windowNamesList.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  std::stringstream wildBuf;
  std::stringstream windBuf;
  wildBuf << wildcardsFile.rdbuf();
  windBuf << windowNamesFile.rdbuf();
  const std::string argh1 = wildBuf.str();
  const std::string argh2 = windBuf.str();
  const char* wildText = argh1.c_str();
  const char* windText = argh2.c_str();
  wildcardsBuf.text(wildText);
  windNamesBuf.text(windText);

  
  Fl_Window* w;
  mainWindow->SaveBlocklistButton->callback(save_blocklists);
  mainWindow->BlockedWildcardsEdit->buffer(wildcardsBuf);
  mainWindow->BlockedWindownamesEdit->buffer(windNamesBuf);
  mainWindow->ToggleNMLButton->callback(toggleNMLEnable);
  mainWindow->CurrentWindowClassOutput->value("(select a window)");
  mainWindow->CurrentWindowNameOutput->value("(select a window)");

  w = mainWindow->NMLRootWindow;
  w->show(argc, argv);
  
  Fl::add_timeout(WINDOW_CHECK_TIMES_PER_SECOND, doThingWithWindows);
  Fl::scheme("gtk+");
  loadColorScheme();
  Fl::reload_scheme(); // if plastic/gtk+/etc, update with new colors 
  return Fl::run();
}
