#ifndef x11_window_manager_h
#define x11_window_manager_h

#include <X11/Xlib.h>           // `apt-get install libx11-dev`
#include <X11/Xmu/WinUtil.h>    // `apt-get install libxmu-dev`
#include <stdlib.h>
#include <iostream>
#include <fstream>
// class main_window {
// public:
//   main_window();
//   Fl_Window *NMLRootWindow;
//   Fl_Button *ToggleNMLButton;
//   Fl_Text_Editor *BlockedWindownamesEdit;
//   Fl_Text_Editor *BlockedWildcardsEdit;
//   Fl_Button *OpenUpdatePage;
// private:
//   inline void cb_OpenUpdatePage_i(Fl_Button*, void*);
//   static void cb_OpenUpdatePage(Fl_Button*, void*);
// public:
//   Fl_Button *OpenAboutPage;
//   Fl_Button *SaveBlocklistButton;
// };
class x11_window_manager {
    public:
        x11_window_manager();
        std::string active_window_name;
        std::string active_window_class;
        int focusedWindowX;
        int focusedWindowY;
        int focusedWindowWidth;
        int focusedWindowHeight;
        bool update(); // returns true if successful
        void minimize_window();
    private:
        Display* d;
        Window w;
};

#endif