#ifndef DUMMY_WINDOW_MANAGER_H
#define DUMMY_WINDOW_MANAGER_H
#include <stdlib.h>
#include <iostream>
#include <fstream>

class dummy_window_manager {
    public:
        dummy_window_manager();
        std::string active_window_name = "HELLO,";
        std::string active_window_class = "I AM A DUMMY WINDOW MANAGER";
        int focusedWindowX = 69;
        int focusedWindowY = 69;
        int focusedWindowWidth = 69;
        int focusedWindowHeight = 69;
        bool update(); // returns true if successful
        bool is_currently_active_window_owned_by_us();
        void toggle_window_state(bool shouldUpdate);
        void minimize_active_window();
        void show_active_window();
    private:
        bool windowIsMinimized = false;
};

#endif
