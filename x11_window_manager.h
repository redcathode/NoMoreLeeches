#ifndef x11_window_manager_h
#define x11_window_manager_h

#include <X11/Xlib.h>           // `apt-get install libx11-dev`
#include <X11/Xmu/WinUtil.h>    // `apt-get install libxmu-dev`
#include <stdlib.h>
#include <FL/Fl.H>
#include <iostream>
#include <fstream>
#include <FL/Fl.H>
#include <FL/x.H>

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
        bool is_currently_active_window_owned_by_us();
        void toggle_window_state(bool shouldUpdate);
        void minimize_active_window();
        void show_active_window();
        void bring_nml_hidden_to_front(Fl_Window* win);
    private:
        Display* d;
        Window w;
        bool windowIsMinimized;
};

#endif
