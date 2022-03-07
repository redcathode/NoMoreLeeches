#include "x11_window_manager.h"
#include "xlib_window_grab.h"

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
            int winPos[2]; 
            Window root;
            root =  XRootWindow(d, 0);
            Window child;
            XTranslateCoordinates(d, w, root, 0, 0, &focusedWindowX, &focusedWindowY, &child);
            focusedWindowWidth = xwa.width;
            focusedWindowHeight = xwa.height;
            // std::cout << "X" << xwa.x << std::endl;
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
