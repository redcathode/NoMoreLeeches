#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem> // or #include <filesystem> for C++17 and up
#include <algorithm>

    
namespace fs = std::experimental::filesystem;


#include "main_window.h"
#include "blocked_window.h"
#include "theme_manager.h"
#include "select_time_popup.h"
#define WINDOW_CHECK_TIMES_PER_SECOND 0.1
#ifdef _WIN32
#define DIRSEP "\\"
#include "win32_window_manager.h"
win32_window_manager *windowManager = new win32_window_manager();
#else
#define DIRSEP "/"
#include "x11_window_manager.h"
x11_window_manager *windowManager = new x11_window_manager();
#endif

Fl_Text_Buffer wildcardsBuf;
Fl_Text_Buffer windNamesBuf;
std::string configPath;
main_window *mainWindow = new main_window();
select_time_popup *selectTimePopup = nullptr;
bool currentlyBlocking = false;
blocked_window *blockedWindow = nullptr;
int defaultSecondsLeft = 60;
int currentSecondsLeft = defaultSecondsLeft;
bool timerIsStopped = true;
bool currentlyInLockdownMode = false;
int lockdownModeSecondsLeft = 0;
int currentColorScheme = 0;
std::string currentOkClass = "hopefully there isn't a window matching this text exactly";

int violations = 0;
bool nmlActive = false;
bool brieflyShowActive = false;
void writeStringToFile(std::string& string, std::string& file) {
  std::ofstream fileOutputStream;
  fileOutputStream.open(configPath + DIRSEP + file, std::fstream::out | std::fstream::trunc);
  fileOutputStream << string;
  fileOutputStream.close();
}
char* strToChar(std::string& str) {
//     std::vector<char> output(str.c_str(), str.c_str() + str.size() + 1);
//     char *returnChar = reinterpret_cast<char*>(output.data());
//     return returnChar;
  str = str + "\0";
  char *returnChar = new char[str.size() + 1];
  std::copy(str.begin(), str.end(), returnChar);
  return returnChar;
}
bool str_replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
void setTimer(int time) {
  std::string timerText;
  if (time > 0) {
    timerText = "If you really want to access this window,\nwait secks:flushed: seconds.";
    std::string timerNum = std::to_string(time);
    str_replace(timerText, "secks:flushed:", timerNum);
  } else {
    timerText = "You can't access this window.";
  }
  char *timerChar = strToChar(timerText);
  blockedWindow->WaitDisplay->label(timerChar);
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
bool isStringInWildcards(std::string& windowName) {
  std::vector<std::string> wildcardList = split_string(wildcardsBuf.text(), "\n");
  for (std::string wildcardToCheck : wildcardList) {
    //std::cout << "Checking " << wildcardToCheck << " against " << windowName << std::endl;
    if (strstr(windowName.c_str(), wildcardToCheck.c_str()) && wildcardToCheck != "") {
      return true;
    }
  }
  return false;
}
void deleteBlockedWindow() {
  std::cout << windowManager->active_window_class << ":" << windowManager->active_window_name << ":" << windowManager->is_currently_active_window_owned_by_us() << std::endl;
  if (blockedWindow != nullptr) { // this prevents a segfault if the function is called and blockedWindow is already deleted
    blockedWindow->BlockWindow->hide();
    delete blockedWindow;
    blockedWindow = nullptr;
  }
}
void minimizeWindowTimeout(void*) {
//     blockedWindow->BlockWindow->show();
//     blockedWindow->BlockWindow->resize(windowManager->focusedWindowX, windowManager->focusedWindowY, windowManager->focusedWindowWidth, windowManager->focusedWindowHeight);
//     if (nmlActive) {
//       windowManager->minimize_active_window();
//     }
    //deleteBlockedWindow();
    brieflyShowActive = false;
}
void showBrieflyCallback(Fl_Widget*) {
    windowManager->show_active_window();
    std::cout << "deleting blocked window because show briefly callback" << std::endl;
    deleteBlockedWindow();
    currentSecondsLeft = defaultSecondsLeft;
    Fl::add_timeout(5.0, minimizeWindowTimeout);
    brieflyShowActive = true;;
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
    currentSecondsLeft = defaultSecondsLeft;
    std::cout << "resetting timer";
  }
  //blockedWindow->WaitDisplay->label();
  setTimer(currentSecondsLeft);
  if (currentSecondsLeft == 0) {
    currentSecondsLeft = defaultSecondsLeft;
    currentOkClass = windowManager->active_window_class;
    std::cout << "deleting blocked window because timer is at 0" << std::endl;
    deleteBlockedWindow();
    windowManager->show_active_window();
    //windowManager->show_active_window();
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
  std::string delayStr = mainWindow->BlockedWindowDelayInput->value();
  if (delayStr != "") {
    defaultSecondsLeft = std::stoi(delayStr);
    std::string filename = "delaySecs.txt";
    writeStringToFile(delayStr, filename);
  }
  Fl::add_timeout(0.7, blocklistSavedCallback);
}
void toggleEnabled() {
  nmlActive = !nmlActive;
  if (nmlActive) {
    mainWindow->ToggleNMLButton->label("Disable NML\n(currently enabled)");
  } else {
    mainWindow->ToggleNMLButton->label("Enable NML\n(currently disabled)");
  }
}
void toggleNMLEnable(Fl_Widget*) {
  toggleEnabled();
}
void deleteSelectTimePopup() {
  selectTimePopup->SelectTimePopup->hide();
  delete selectTimePopup;
  selectTimePopup = nullptr;
}
void updateLockdownModeCountdown() {
  int hours, minutes, seconds;
  seconds = lockdownModeSecondsLeft;
  minutes = seconds / 60;
  hours = minutes / 60;

  std::stringstream tempName;
  tempName << hours << " hours, " << minutes << " minutes, " << int(seconds % 60) << " seconds left." << std::endl;
  std::string evenMoreTempName = tempName.str();
  selectTimePopup->WaitDisplay->label(strToChar(evenMoreTempName));
}
void decreaseTimeByOne(void*) {
  lockdownModeSecondsLeft -= 1;
  if (lockdownModeSecondsLeft == 0) {
    mainWindow->NMLRootWindow->show();
    toggleEnabled();
    deleteSelectTimePopup();
  } else {
    updateLockdownModeCountdown();
    Fl::repeat_timeout(1.0, decreaseTimeByOne);
  }
}
void lockdownWindowCloseAttemptCallback(Fl_Widget* widget, void*) {
}
void selectTimePopupWasOk(Fl_Widget*) {
  std::cout << "hrs: " << selectTimePopup->LockdownHoursInput->value() << " mins: " << selectTimePopup->LockdownMinutesInput->value() << std::endl;
  std::string setHours = selectTimePopup->LockdownHoursInput->value();
  std::string setMinutes = selectTimePopup->LockdownMinutesInput->value();
  if (setMinutes != "" && setHours != "") {
    int hours = std::stoi(setHours);
    int minutes = std::stoi(setMinutes);
    if (minutes >= 0 && hours >= 0 && hours + minutes > 0) {
      lockdownModeSecondsLeft = (hours * 3600) + (minutes * 60);
      std::cout << lockdownModeSecondsLeft << std::endl;
      selectTimePopup->Cancel->hide();
      selectTimePopup->OK->hide();
      selectTimePopup->LockdownHoursInput->hide();
      selectTimePopup->LockdownMinutesInput->hide();
      selectTimePopup->MinutesDisplay->hide();
      selectTimePopup->WaitDisplay->show();
      selectTimePopup->SelectTimePopup->label("NML: Lockdown");
      mainWindow->NMLRootWindow->hide();
      selectTimePopup->SelectTimePopup->callback(lockdownWindowCloseAttemptCallback);
      updateLockdownModeCountdown();
      nmlActive = true;
      Fl::add_timeout(1.0, decreaseTimeByOne);
    }
  }
}
void enterLockdownMode(Fl_Widget*) {
  if (selectTimePopup != nullptr) {
    deleteSelectTimePopup();
  }
  selectTimePopup = new select_time_popup();
  selectTimePopup->make_window();
  selectTimePopup->SelectTimePopup->show();
  selectTimePopup->OK->callback(selectTimePopupWasOk);
  selectTimePopup->LockdownHoursInput->value("0");
  selectTimePopup->LockdownMinutesInput->value("0");
}
void blockingWindowCheckTimer(void*) {
  if (!windowManager->is_currently_active_window_owned_by_us()) {
    std::cout << "deleting blocked window because currently active window isn't owned by us" << std::endl;
    deleteBlockedWindow();
  } else {
    Fl::repeat_timeout(WINDOW_CHECK_TIMES_PER_SECOND, blockingWindowCheckTimer);
  }
}
void makeBlockedWindow(bool wasBlockedBecauseClass) {
  if (!brieflyShowActive) {
      currentSecondsLeft = defaultSecondsLeft;
      currentlyBlocking = true;
      blockedWindow = new blocked_window();
      blockedWindow->make_window();
      blockedWindow->BlockWindow->resize(windowManager->focusedWindowX, windowManager->focusedWindowY, windowManager->focusedWindowWidth, windowManager->focusedWindowHeight);
      std::cout << windowManager->focusedWindowX << ":" << windowManager->focusedWindowY << ":" << windowManager->focusedWindowWidth << ":"  << windowManager->focusedWindowHeight <<  std::endl;
      std::string applicationName = windowManager->active_window_name;
      std::string applicationGame = windowManager->active_window_class;
      std::string reasonText;
      if (wasBlockedBecauseClass) {
        reasonText = "Reason: 'windowclass' matched item in blocked window class list";
        str_replace(reasonText, "windowclass", applicationGame);
      } else {
        reasonText = "Reason: 'windowname' matched item in blocked wildcard list";
        str_replace(reasonText, "windowname", applicationName);
      }
      std::string windowText = "Window 'windowname' blocked.";
      setTimer(currentSecondsLeft);

      str_replace(windowText, "windowname", applicationName);
      char *windowChar = strToChar(windowText);
      char *reasonChar = strToChar(reasonText);
      //std::vector<char> reason(reasonText.c_str(), reasonText.c_str() + reasonText.size() + 1);
      //std::vector<char> window(windowText.c_str(), windowText.c_str() + windowText.size() + 1);
      //char *windowChar = reinterpret_cast<char*>(window.data());
      //char *reasonChar = reinterpret_cast<char*>(reason.data());

//       char *windowChar = new char[windowText.size() + 1];
//       std::copy(windowText.begin(), windowText.end(), windowChar);
      blockedWindow->ReasonDisplay->label(reasonChar);
      blockedWindow->WindowHiddenDisplay->label(windowChar);
      //blockedWindow->make_window(0, 0, 250, 250);
      //std::cout << windowManager->focusedWindowX << std::endl;
      windowManager->minimize_active_window();
      //blockedWindow->BlockWindow->set_non_modal();
      //blockedWindow->BlockWindow->border(0);
      // blockedWindow->BlockWindow->fullscreen();
      //blockedWindow->BlockWindow->set_non_modal();
      blockedWindow->BlockWindow->show();
      windowManager->bring_nml_hidden_to_front(blockedWindow->BlockWindow);
      blockedWindow->ShowBrieflyButton->callback(showBrieflyCallback);
      Fl::remove_timeout(tickWindowShowTimer);
      Fl::add_timeout(1.0, tickWindowShowTimer);
      Fl::add_timeout(WINDOW_CHECK_TIMES_PER_SECOND, blockingWindowCheckTimer);
  }
}
std::string getConfigDir() {
  std::string path;
  if (const char* env_p = std::getenv("APPDATA")) {
        std::cout << "Config dir: " << env_p << '\n';
        path.append(env_p);
        path.append("\\nomoreleeches");
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
  if (!fs::exists(path)) { // Check if src folder exists
    fs::create_directory(path); // create src folder
  }
  std::cout << "Config dir: " << path << std::endl;
  return path;
}
void doThingWithWindows(void*) {
  if (!nmlActive) {
    Fl::repeat_timeout(WINDOW_CHECK_TIMES_PER_SECOND, doThingWithWindows);
    return;
  }
  //std::cout << "current ok class: " << currentOkClass << std::endl;
  if (blockedWindow == nullptr) {
    bool status = windowManager->update();

    if (status && (windowManager->active_window_class == currentOkClass || windowManager->is_currently_active_window_owned_by_us())) {
      //std::cout << "active window class: " << windowManager->active_window_class << std::endl;
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

  std::vector<std::string> classBlockList = split_string(windNamesBuf.text(), "\n");
  if (std::count(classBlockList.begin(), classBlockList.end(), windowManager->active_window_class)) {
    //std::cout << "h" << std::endl;
    //
    if (blockedWindow == nullptr && status) {
      makeBlockedWindow(true);


      // Fl::grab(blockedWindow->BlockWindow);
    }
  }
  if (isStringInWildcards(windowManager->active_window_name)) {
    if (blockedWindow == nullptr && status) {
      makeBlockedWindow(false);


      // Fl::grab(blockedWindow->BlockWindow);
    }
  }

   } //else { // commenting this out as we should just keep the last window that was grabbed successfully
  //   mainWindow->CurrentWindowClassOutput->value("(error getting window name)");
  //   mainWindow->CurrentWindowNameOutput->value("(error getting window name)");
  // }

  Fl::repeat_timeout(WINDOW_CHECK_TIMES_PER_SECOND, doThingWithWindows);
}

void set_theme_from_dropdown(Fl_Widget* w, long i) {
  theme_manager::loadColorScheme(mainWindow->ColorThemeChooser->value());
  std::cout << "theme set to " << mainWindow->ColorThemeChooser->value() << std::endl;
  std::string filename = "theme.txt";
  std::string intToWrite = std::to_string(mainWindow->ColorThemeChooser->value());
  writeStringToFile(intToWrite, filename);
}
int main(int argc, char **argv) {
  configPath = getConfigDir();
  std::ofstream wildcardsFile;
  std::ofstream windowNamesFile;
  std::ofstream themeFile;
  std::ofstream delayFile;
  wildcardsFile.open(configPath + DIRSEP + "wildcardsList.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  windowNamesFile.open(configPath + DIRSEP + "windowNamesList.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  themeFile.open(configPath + DIRSEP + "theme.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  delayFile.open(configPath + DIRSEP + "delaySecs.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  std::stringstream wildBuf;
  std::stringstream windBuf;
  std::stringstream themeBuf;
  std::stringstream delayBuf;
  themeBuf << themeFile.rdbuf();
  delayBuf << delayFile.rdbuf();
  wildBuf << wildcardsFile.rdbuf();
  windBuf << windowNamesFile.rdbuf();
  std::string themeStr = themeBuf.str();
  std::string delayStr = delayBuf.str();
  if (delayStr != "") {
    defaultSecondsLeft = std::stoi(delayStr);
  }
  std::string delayString = std::to_string(defaultSecondsLeft);
  mainWindow->BlockedWindowDelayInput->value(strToChar(delayString));
  if (themeStr != "") {
    currentColorScheme = std::stoi(themeStr);
    mainWindow->ColorThemeChooser->value(currentColorScheme);
  }
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
  mainWindow->ColorThemeChooser->callback(set_theme_from_dropdown);
  mainWindow->CurrentWindowClassOutput->value("(NML is disabled or no window is selected)");
  mainWindow->CurrentWindowNameOutput->value("(NML is disabled or no window is selected)");
  mainWindow->LockdownButton->callback(enterLockdownMode);
  w = mainWindow->NMLRootWindow;
  toggleEnabled();
  w->show(argc, argv);
  
  Fl::add_timeout(WINDOW_CHECK_TIMES_PER_SECOND, doThingWithWindows);
  Fl::scheme("gtk+");
  theme_manager::loadColorScheme(currentColorScheme);
  return Fl::run();
}
