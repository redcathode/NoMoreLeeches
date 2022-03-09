#ifndef WIN32_WINDOW_MANAGER_H
#define WIN32_WINDOW_MANAGER_H
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <FL/Fl.H>
#include <FL/x.H>

class win32_window_manager {
    public:
        win32_window_manager();
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
        bool windowIsMinimized = false;
        HWND currentWindow;
        void bringWindowToFront(HWND winapiWindow);
};

#endif
