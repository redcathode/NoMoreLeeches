// START main.cxx
#include <FL/Fl.H> // include the main FLTK GUI library
#include <FL/Fl_Window.H> // regular windows
#include <FL/Fl_Box.H> // rectangular boxes
#include <FL/fl_ask.H> // popups
#include <stdlib.h> // c++ standard library
#include <iostream> // reading and writing to files and strings
#include <fstream> // same as above
#include <experimental/filesystem> // interfacing with the config files - #include <filesystem> for C++17 and up
#include <algorithm> // searching through strings

    
namespace fs = std::experimental::filesystem; // easier to type


#include "main_window.h" // code for the app's main window
#include "blocked_window.h" // code for the app's 'application blocked' window
#include "theme_manager.h" // code for selecting color themes
#include "select_time_popup.h"
#define WINDOW_CHECK_INTERVAL 0.1 // the interval at which the app checks for windows that should be blocked
#ifdef _WIN32 // if on Windows, use the win32_window_manager to create the windowManager object that'll be used to detect blocked windows
#define DIRSEP "\\"
#include "win32_window_manager.h"
win32_window_manager *windowManager = new win32_window_manager();
#else
#define DIRSEP "/"
#include "x11_window_manager.h" // otherwise, use the x11_window_manager object to do the same
x11_window_manager *windowManager = new x11_window_manager();
#endif

Fl_Text_Buffer wildcardsBuf; // text buffer for the editable list of window wildcards
Fl_Text_Buffer windNamesBuf; // text buffer for the editable list of window classes
std::string configPath; // the path to the app's configuration folder
main_window *mainWindow = new main_window(); // the main window (an instance of main_window.cxx)
select_time_popup *selectTimePopup = nullptr; // an instance of select_time_popup.cxx, currently not initialized
bool currentlyBlocking = false; // whether or not the app is currently blocking windows
blocked_window *blockedWindow = nullptr; // same as select_time_popup but for blocked_window.cxx
int defaultSecondsLeft = 60; // the default amount of seconds that a window will be blocked for (60)
int currentSecondsLeft = defaultSecondsLeft; // the current amount of seconds left on the block timer, defaults to defaultSecondsLeft (duh)
bool timerIsStopped = true; // whether or not the currentSecondsLeft timer is stopped
bool currentlyInLockdownMode = false; // whether or not the app is currently in lockdown mode
int lockdownModeSecondsLeft = 0; // seconds left in lockdown mode
int currentColorScheme = 0; // index of the current color scheme (0-4, see theme_manager.cxx and theme_manager.h)
std::string currentOkClass = "hopefully there isn't a window matching this text exactly"; // the window that's currently exempt from blocking. Used to prevent the app from instantly blocking a window after it's shown. This is set after a window's block timer expires and changed after the user defocuses the window

// int violations = 0;
bool nmlActive = false; // whether or not the app is currently enabled
bool brieflyShowActive = false; // whether or not the "show window for 5 seconds" timer is active

std::vector<std::string> wildcardList; // the list of blocked wildcards
std::vector<std::string> classBlockList;


// write a std::string to a file
void writeStringToFile(std::string& string, std::string& file) { 
  std::ofstream fileOutputStream;
  fileOutputStream.open(configPath + DIRSEP + file, std::fstream::out | std::fstream::trunc);
  fileOutputStream << string;
  fileOutputStream.close();
}

// convert a std::string to a char*
char* strToChar(std::string& str) {
//     std::vector<char> output(str.c_str(), str.c_str() + str.size() + 1);
//     char *returnChar = reinterpret_cast<char*>(output.data());
//     return returnChar;
  char *returnChar = new char[1024];
  strcpy(returnChar, str.c_str());
  return returnChar;
}

// replace a phrase (from) with another phrase (to) within a string (str)
bool str_replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// set the 'If you want to access this window, wait X seconds' text, where time is X
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

// split a string into a vector of strings given the string and the delimiter to split it by
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter) {
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

// check if a std::string exists within the list of window wildcards
bool isStringInWildcards(std::string& windowName) {
  for (std::string wildcardToCheck : wildcardList) {
    //std::cout << "Checking " << wildcardToCheck << " against " << windowName << std::endl;
    if (strstr(windowName.c_str(), wildcardToCheck.c_str()) && wildcardToCheck != "") {
      return true;
    }
  }
  return false;
}

// delete the blockedWindow object and hide the window associated with it, freeing it for future use
void deleteBlockedWindow() {
  std::cout << windowManager->active_window_class << ":" << windowManager->active_window_name << ":" << windowManager->is_currently_active_window_owned_by_us() << std::endl;
  if (blockedWindow != nullptr) { // this prevents a segfault if the function is called and blockedWindow is already deleted
    blockedWindow->BlockWindow->hide();
    delete blockedWindow;
    blockedWindow = nullptr;
  }
}

// set brieflyShowActive to false. This is called 5 seconds after the 'Briefly show window' button is pressed.
void minimizeWindowTimeout(void*) {
//     blockedWindow->BlockWindow->show();
//     blockedWindow->BlockWindow->resize(windowManager->focusedWindowX, windowManager->focusedWindowY, windowManager->focusedWindowWidth, windowManager->focusedWindowHeight);
//     if (nmlActive) {
//       windowManager->minimize_active_window();
//     }
    //deleteBlockedWindow();
    brieflyShowActive = false;
}

// callback to briefly show the window, called when the 'show window briefly' button is pressed
void showBrieflyCallback(Fl_Widget*) {
    windowManager->show_active_window();
    std::cout << "deleting blocked window because show briefly callback" << std::endl;
    deleteBlockedWindow();
    currentSecondsLeft = defaultSecondsLeft;
    Fl::add_timeout(5.0, minimizeWindowTimeout);
    brieflyShowActive = true;;
}

// When the 'save blocklists' button is pressed, it temporarily changes the button's title to "Saved". This changes it back.
void blocklistSavedCallback(void*) {
  mainWindow->SaveBlocklistButton->label("Save Blocklists");
}

// This function 'ticks' the timer that shows the window after defaultSecondsLeft seconds
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

// This saves the lists of blocked window names and classes to .txt files in the app's config dir.
void save_blocklists(Fl_Widget*) {
  wildcardList = split_string(wildcardsBuf.text(), "\n");
  classBlockList = split_string(windNamesBuf.text(), "\n");

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

// Toggle NML being enabled.
void toggleEnabled() {
  nmlActive = !nmlActive;
  if (nmlActive) {
    mainWindow->ToggleNMLButton->label("Disable NML\n(currently enabled)");
  } else {
    mainWindow->ToggleNMLButton->label("Enable NML\n(currently disabled)");
  }
}

// Allows the above function to be called from an FLTK callback.
void toggleNMLEnable(Fl_Widget*) {
  toggleEnabled();
}

// Hides, deletes, and frees selectTimePopup
void deleteSelectTimePopup() {
  selectTimePopup->SelectTimePopup->hide();
  delete selectTimePopup;
  selectTimePopup = nullptr;
}

// sets the lockdown mode's window's timer text. 
void updateLockdownModeCountdown() {
  int hours, minutes, seconds;
  seconds = lockdownModeSecondsLeft;
  minutes = seconds / 60;
  hours = minutes / 60;

  std::stringstream tempName;
  tempName << int(hours % 60) << " hours, " << int(minutes % 60) << " minutes, " << int(seconds % 60) << " seconds left.";
  std::string evenMoreTempName = tempName.str();
  selectTimePopup->WaitDisplay->label(strToChar(evenMoreTempName));
}

// Decreases the lockdown mode timer's seconds. Called when lockdown mode is enabled, and calls itself every second until the timer finishes.
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

// FLTK window close callback that doesn't do anything. Used by the lockdown mode window.
void lockdownWindowCloseAttemptCallback(Fl_Widget* widget, void*) {}

// If lockdown mode is started with a valid time, this function is called to hide the other windows and start lockdown.
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

// This function opens the popup that allows the user to select the time to enter lockdown mode for
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

// If the blocked window is active, this will be called every WINDOW_CHECK_INTERVAL seconds. Will hide the window if it's not focused.
void blockingWindowCheckTimer(void*) {
  if (!windowManager->is_currently_active_window_owned_by_us()) {
    std::cout << "deleting blocked window because currently active window isn't owned by us" << std::endl;
    deleteBlockedWindow();
  } else {
    Fl::repeat_timeout(WINDOW_CHECK_INTERVAL, blockingWindowCheckTimer);
  }
}

// This function will make and show the blocked window. The boolean input tells it whether the window was blocked because of its class or its name.
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
      Fl::add_timeout(WINDOW_CHECK_INTERVAL, blockingWindowCheckTimer);
  }
}

// Gets the app's config dir, returns a std::string
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

// This is the function that actually checks if a window should be blocked, and pops up the blocked window if it does.
void doThingWithWindows(void*) {
  if (!nmlActive) {
    Fl::repeat_timeout(WINDOW_CHECK_INTERVAL, doThingWithWindows);
    return;
  }
  //std::cout << "current ok class: " << currentOkClass << std::endl;
  if (blockedWindow == nullptr) {
    bool status = windowManager->update();

    if (status && (windowManager->active_window_class == currentOkClass || windowManager->is_currently_active_window_owned_by_us())) {
      //std::cout << "active window class: " << windowManager->active_window_class << std::endl;
      Fl::repeat_timeout(WINDOW_CHECK_INTERVAL, doThingWithWindows);
      return;
    } else {
      currentOkClass = "hopefully there isn't a window matching this text exactly";
    }
    // std::cout << windowManager->active_window_class << std::endl;
    if (windowManager->active_window_class != "FLTK") { // todo: this will also whitelist other fltk apps. unimportant, since no apps that people actually use are written in fltk
      mainWindow->CurrentWindowClassOutput->value(windowManager->active_window_class.c_str());
      mainWindow->CurrentWindowNameOutput->value(windowManager->active_window_name.c_str());
    }

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

  Fl::repeat_timeout(WINDOW_CHECK_INTERVAL, doThingWithWindows);
}

// This callback is called when the theme dropdown has an item selected. It calls on functions in theme_manager.cxx that change the color scheme, passing the dropdown's selected index as a parameter.
void set_theme_from_dropdown(Fl_Widget* w, long i) {
  theme_manager::loadColorScheme(mainWindow->ColorThemeChooser->value());
  std::cout << "theme set to " << mainWindow->ColorThemeChooser->value() << std::endl;
  std::string filename = "theme.txt";
  std::string intToWrite = std::to_string(mainWindow->ColorThemeChooser->value());
  writeStringToFile(intToWrite, filename);
}

// The app's main method.
int main(int argc, char **argv) {
  // This snippet opens the app's config files and reads their values into variables.
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

  // This snippet creates the blocked window, sets callbacks, sets the text editing buffers to point to local variables, sets default values, and shows the window.
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
  
  // Finally, we set the configured color scheme and set the widget scheme to GTK+, start the window check timer, and then run the application.
  Fl::add_timeout(WINDOW_CHECK_INTERVAL, doThingWithWindows);
  Fl::scheme("gtk+");
  theme_manager::loadColorScheme(currentColorScheme);

  wildcardList = split_string(wildcardsBuf.text(), "\n"); // populate the wildcard list and class blocklist from the text of the editable buffers
  classBlockList = split_string(windNamesBuf.text(), "\n");

  return Fl::run();
}
// END MAIN.CXX
// START BLOCKED_WINDOW.CXX

Fl_Window* blocked_window::make_window() { // Generate the "Application Blocked" window.
  { BlockWindow = new Fl_Window(420, 420, "NML: Application Hidden");
    BlockWindow->user_data((void*)(this));
    { ReasonDisplay = new Fl_Text_Display(185, 145, 50, 30, "Reason:");
      ReasonDisplay->box(FL_NO_BOX);
    } // Fl_Text_Display* ReasonDisplay
    { WindowHiddenDisplay = new Fl_Text_Display(185, 120, 50, 30, "Window \'\' hidden.");
      WindowHiddenDisplay->box(FL_NO_BOX);
    } // Fl_Text_Display* WindowHiddenDisplay
    { WaitDisplay = new Fl_Text_Display(185, 207, 50, 30, "If you really want to access this window, \nwait 60 seconds.");
      WaitDisplay->box(FL_NO_BOX);
    } // Fl_Text_Display* WaitDisplay
    { ShowBrieflyButton = new Fl_Button(100, 235, 215, 75, "Show window for 5 seconds");
    } // Fl_Button* ShowBrieflyButton
    BlockWindow->end();
    BlockWindow->resizable(BlockWindow);
  } // Fl_Window* BlockWindow
  return BlockWindow;
}

// END BLOCKED_WINDOW.CXX
// START MAIN_WINDOW.CXX

// Open the app's update page.
void main_window::cb_OpenUpdatePage_i(Fl_Button*, void*) {
  fl_open_uri("https://github.com/asolidtime/nomoreleeches/releases");
}
// Callback that is called when the 'Open Update Page' button is clicked. Calls the above function
void main_window::cb_OpenUpdatePage(Fl_Button* o, void* v) {
  ((main_window*)(o->parent()->user_data()))->cb_OpenUpdatePage_i(o,v);
}
// Same as OpenUpdatePage_i, but for the about page
void main_window::cb_OpenAboutPage_i(Fl_Button*, void*) {
  fl_open_uri("https://github.com/asolidtime/nomoreleeches/");
}
// See above comment.
void main_window::cb_OpenAboutPage(Fl_Button* o, void* v) {
  ((main_window*)(o->parent()->user_data()))->cb_OpenAboutPage_i(o,v);
}

Fl_Menu_Item main_window::menu_ColorThemeChooser[] = { // Theme chooser dialog. Creates 5 items with titles of themes and 1 without.
 {"Dark", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"Black", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"Gray", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"Tan", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {"Shake", 0,  0, 0, 0, (uchar)FL_NORMAL_LABEL, 0, 14, 0},
 {0,0,0,0,0,0,0,0,0}
};

main_window::main_window() { // Generate the app's main window.
  { NMLRootWindow = new Fl_Window(315, 555, "NoMoreLeeches v0.1");
    NMLRootWindow->labelfont(4);
    NMLRootWindow->user_data((void*)(this));
    { ToggleNMLButton = new Fl_Button(10, 10, 140, 60, "Enable NML\n(currently disabled)");
      ToggleNMLButton->tooltip("Disable or enable NML.");
    } // Fl_Button* ToggleNMLButton
    { BlockedWindownamesEdit = new Fl_Text_Editor(10, 150, 295, 140, "Blocked Window Classes (one per line)");
      BlockedWindownamesEdit->tooltip("If a window\'s class is in this list, it\'ll be blocked.");
      BlockedWindownamesEdit->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Text_Editor* BlockedWindownamesEdit
    { BlockedWildcardsEdit = new Fl_Text_Editor(10, 312, 295, 130, "Blocked Wildcards (one per line)");
      BlockedWildcardsEdit->tooltip("If any of these strings are found in a window\'s name, that window will be bl\
ocked.");
      BlockedWildcardsEdit->align(Fl_Align(FL_ALIGN_TOP_LEFT));
    } // Fl_Text_Editor* BlockedWildcardsEdit
    { OpenUpdatePage = new Fl_Button(165, 10, 140, 20, "Open Update Page");
      OpenUpdatePage->tooltip("Opens NML\'s download page. Make sure to get the newest version!");
      OpenUpdatePage->callback((Fl_Callback*)cb_OpenUpdatePage);
    } // Fl_Button* OpenUpdatePage
    { OpenAboutPage = new Fl_Button(165, 30, 140, 20, "GitHub page");
      OpenAboutPage->tooltip("Show information about NML.");
      OpenAboutPage->callback((Fl_Callback*)cb_OpenAboutPage);
    } // Fl_Button* OpenAboutPage
    { SaveBlocklistButton = new Fl_Button(165, 50, 140, 20, "Save Block Options");
    } // Fl_Button* SaveBlocklistButton
    { Fl_Text_Display* o = new Fl_Text_Display(10, 457, 50, 30, "Window Name:");
      o->box(FL_NO_BOX);
      o->labelsize(13);
      o->align(Fl_Align(FL_ALIGN_BOTTOM_LEFT));
    } // Fl_Text_Display* o
    { Fl_Text_Display* o = new Fl_Text_Display(10, 486, 50, 30, "Window Class:");
      o->box(FL_NO_BOX);
      o->labelsize(13);
      o->align(Fl_Align(FL_ALIGN_BOTTOM_LEFT));
    } // Fl_Text_Display* o
    { CurrentWindowNameOutput = new Fl_Output(115, 483, 189, 28);
      CurrentWindowNameOutput->labeltype(FL_NO_LABEL);
      CurrentWindowNameOutput->labelfont(4);
      CurrentWindowNameOutput->textfont(4);
      CurrentWindowNameOutput->textsize(10);
    } // Fl_Output* CurrentWindowNameOutput
    { CurrentWindowClassOutput = new Fl_Output(115, 512, 189, 28);
      CurrentWindowClassOutput->labeltype(FL_NO_LABEL);
      CurrentWindowClassOutput->labelfont(4);
      CurrentWindowClassOutput->textfont(4);
      CurrentWindowClassOutput->textsize(10);
    } // Fl_Output* CurrentWindowClassOutput
    { ColorThemeChooser = new Fl_Choice(105, 107, 200, 20, "Color Theme:");
      ColorThemeChooser->down_box(FL_BORDER_BOX);
      ColorThemeChooser->menu(menu_ColorThemeChooser);
    } // Fl_Choice* ColorThemeChooser
    { LockdownButton = new Fl_Button(10, 75, 295, 25, "ENTER LOCKDOWN MODE");
    } // Fl_Button* LockdownButton
    { BlockedWindowDelayInput = new Fl_Int_Input(170, 448, 135, 28, "Blocked window delay:");
      BlockedWindowDelayInput->tooltip("The amount of time you\'ll need to wait before a blocked window is shown. Set\
 this to a negative number to hide them forever.");
      BlockedWindowDelayInput->type(2);
    } // Fl_Int_Input* BlockedWindowDelayInput
    NMLRootWindow->end();
  } // Fl_Window* NMLRootWindow
}
// END MAIN_WINDOW.CXX
// START SELECT_TIME_POPUP.CXX
void select_time_popup::cb_Cancel_i(Fl_Button*, void*) { // hides the window.
  SelectTimePopup->hide();
}
void select_time_popup::cb_Cancel(Fl_Button* o, void* v) { // Callback function for the cancel button that calls the above function
  ((select_time_popup*)(o->parent()->user_data()))->cb_Cancel_i(o,v);
}

Fl_Window* select_time_popup::make_window() { // Generate the 'Select Time' popup window.
  { SelectTimePopup = new Fl_Window(425, 125, "Select Time");
    SelectTimePopup->user_data((void*)(this));
    { LockdownHoursInput = new Fl_Int_Input(155, 17, 65, 28, "Enter lockdown for: ");
      LockdownHoursInput->type(2);
    } // Fl_Int_Input* LockdownHoursInput
    { LockdownMinutesInput = new Fl_Int_Input(270, 17, 35, 28, "hours,");
      LockdownMinutesInput->type(2);
    } // Fl_Int_Input* LockdownMinutesInput
    { MinutesDisplay = new Fl_Text_Display(367, 16, 75, 30, "minutes");
      MinutesDisplay->box(FL_NO_BOX);
      MinutesDisplay->align(Fl_Align(FL_ALIGN_LEFT));
    } // Fl_Text_Display* MinutesDisplay
    { OK = new Fl_Button(272, 93, 70, 25, "OK");
    } // Fl_Button* OK
    { Cancel = new Fl_Button(350, 93, 70, 25, "Cancel");
      Cancel->callback((Fl_Callback*)cb_Cancel);
    } // Fl_Button* Cancel
    { WaitDisplay = new Fl_Text_Display(0, 68, 425, 40, "Wait xx hours, yy minutes.");
      WaitDisplay->box(FL_NO_BOX);
      WaitDisplay->hide();
    } // Fl_Text_Display* WaitDisplay
    //SelectTimePopup->set_modal();
    SelectTimePopup->end();
  } // Fl_Window* SelectTimePopup
  return SelectTimePopup;
}
// END SELECT_TIME_POPUP.CXX
// START WIN32_WINDOW_MANAGER.CXX
#include "win32_window_manager.h"

win32_window_manager::win32_window_manager() { // Initialize the class
    // do nothing.
}
bool win32_window_manager::update() { // Update the currently active window title and class
    char wnd_title[256];
    char wnd_class[256];
    RECT tempRect;
    currentWindow = GetForegroundWindow(); // get handle of currently active window
    GetWindowText(currentWindow, wnd_title, sizeof(wnd_title));
    GetClassName(currentWindow, wnd_class, sizeof(wnd_class));
    GetWindowRect(currentWindow, &tempRect);
    std::string tempStr;
    tempStr.assign(wnd_class);
    if (tempStr == "") {
        return false;
    } else {
        active_window_class.assign(wnd_class);
        active_window_name.assign(wnd_title);
        focusedWindowX = tempRect.left;
        focusedWindowY = tempRect.top;
        focusedWindowWidth = tempRect.right - tempRect.left;
        focusedWindowHeight = tempRect.bottom - tempRect.top;
        return true;
    }
}
bool win32_window_manager::is_currently_active_window_owned_by_us() { // Self explanatory.
    HWND temp_win = GetForegroundWindow();
    char temp_win_class[256];
    GetClassName(temp_win, temp_win_class, sizeof(temp_win_class));
    std::string tempStr;

    tempStr.assign(temp_win_class);
    std::cout << tempStr << std::endl;
    if (tempStr == "FLTK") {
        return true;
    } else {
        return false;
    }
}
void win32_window_manager::toggle_window_state(bool shouldUpdate) { // This isn't necessary anymore, kept for unknown reasons

}
void win32_window_manager::minimize_active_window() { // Minimize the currently active window
    PostMessage(currentWindow, WM_SYSCOMMAND, SC_MINIMIZE, 0);
}
void win32_window_manager::show_active_window() { // Unminimize the currently active window
    PostMessage(currentWindow, WM_SYSCOMMAND, SC_RESTORE, 0);
    bringWindowToFront(currentWindow);
}
void win32_window_manager::bring_nml_hidden_to_front(Fl_Window* win) { // Bring the 'Window Hidden' window to front.
    char temp_win_name[256];
    // apparently, the windows API doesn't let users do this, so I have to allow myself to set the foreground window first
    HWND winapiWindow = fl_xid(win);
    GetWindowText(winapiWindow, temp_win_name, sizeof(temp_win_name));
    std::cout << "bringing " << temp_win_name << " to front" << std::endl;
    /*AttachThreadInput(GetWindowThreadProcessId(winapiWindow), GetWindowThreadProcessId(currentWindow))*/;
    //HWND currentlyActiveWindow =GetForegroundWindow();
    //DWORD our_pid = 0;
    //DWORD their_pid = 0;
//     GetWindowThreadProcessId(winapiWindow, &our_pid);
//     GetWindowThreadProcessId(currentlyWindow, &their_pid);

    //AttachThreadInput(our_pid, their_pid, true);
    //std::cout << "our pid: " << our_pid << ", their pid:" << their_pid << std::endl;
    //AllowSetForegroundWindow(our_pid);
    bringWindowToFront(winapiWindow);
    //SetWindowPos(winapiWindow, HWND_TOP, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE  | SWP_NOSIZE);
    SetActiveWindow(winapiWindow);

}
void win32_window_manager::bringWindowToFront(HWND winapiWindow) { // Bring any given window to front.
    SetForegroundWindow(winapiWindow);
    HWND hCurWnd = GetForegroundWindow();
    DWORD dwMyID = GetCurrentThreadId();
    DWORD dwCurID = GetWindowThreadProcessId(hCurWnd, NULL);
    AttachThreadInput(dwCurID, dwMyID, TRUE);
    SetWindowPos(winapiWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    SetWindowPos(winapiWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
    SetForegroundWindow(winapiWindow);
    SetFocus(winapiWindow);
    SetActiveWindow(winapiWindow);
    AttachThreadInput(dwCurID, dwMyID, FALSE);
}
// END WIN32_WINDOW_MANAGER.CXX
// START X11_WINDOW_MANAGER.CXX
// Not gonna comment any of this b/c it all does the same thing as the Windows code, just on a different platform
x11_window_manager::x11_window_manager() {
    setlocale(LC_ALL, ""); // see man locale
    d = open_display();
}

bool x11_window_manager::update() {

    windowIsMinimized = false;
    w = get_active_window(d);
    char* windname;
    char* windclass;
    if (get_window_name(d, w, &windname) != -1) {
        active_window_name.assign(windname);
        if (get_window_class(d, w, &windclass) != -1) {
            active_window_class.assign(windclass);
            XWindowAttributes xwa;
            XGetWindowAttributes(d, w, &xwa);
            Window root;
            root =  XRootWindow(d, 0);
            Window child;
            XTranslateCoordinates(d, w, root, 0, 0, &focusedWindowX, &focusedWindowY, &child);
            focusedWindowWidth = xwa.width;
            focusedWindowHeight = xwa.height;
            // std::cout << "X" << xwa.x << std::endl;
            // std::cout << "[x11_window_manager::update] window class: " << active_window_class << ", window name: " << active_window_name << std::endl;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
bool x11_window_manager::is_currently_active_window_owned_by_us() {
    Window separateWindow = get_active_window(d);
    char* windclass;
    if (get_window_class(d, separateWindow, &windclass) != -1) {
        std::string windowCmp;
        windowCmp.assign(windclass);
        if (windowCmp == "FLTK") {
            return true;
        } else {
            // std::cout << "[x11_window_manager::is_currently_active_window_owned_by_us] currently active window isn't owned by us, it's owned by " << windowCmp << std::endl;
            return false;
        }

    } else {
        return false;
    }
}
void x11_window_manager::minimize_active_window() {
    windowIsMinimized = true;
    minimize_window(w, d);
}
void x11_window_manager::show_active_window() {
    windowIsMinimized = false;
    restore_window(w, d);
    std::cout << "Raising window" << std::endl;
    XRaiseWindow(d, w);
    XFlush(d);
}

void x11_window_manager::toggle_window_state(bool shouldUpdate) {

    if (windowIsMinimized) {
        restore_window(w, d);
    } else {
        minimize_window(w, d);
    }
    windowIsMinimized = !windowIsMinimized;

    //if (shouldUpdate) update();
}
void x11_window_manager::bring_nml_hidden_to_front(Fl_Window* win) {
    restore_window(fl_xid(win), d);
    XRaiseWindow(d, fl_xid(win));
}
// END X11_WINDOW_MANAGER.CXX
// START THEME_MANAGER.CXX

void theme_manager::setColor(int one, int two, int three, int four) { // Set one of the app's colors
  Fl::set_color(Fl_Color(one), two, three, four);
}
void theme_manager::loadColorScheme(int themeIndex) {  // Load a color scheme given a number from 0 to 4. These are taken from the app's theme dropdown.
   switch(themeIndex) { // 0=dark, 1=black, 2=gray, 3=tan, 4=shake
       case 0:
           loadDarkColorScheme();
           break;
       case 1:
           loadBlackColorScheme();
           break;
       case 2:
           loadGrayColorScheme();
           break;
       case 3:
           loadTanColorScheme();
           break;
       case 4:
           loadShakeColorScheme();
           break;
   }
   Fl::reload_scheme(); // if plastic/gtk+/etc, update with new colors
}
// I'm not including the rest of the file, as all the theme code is about 800 lines of setColor over and over again.
// END THEME_MANAGER.CXX