#include "x11_window_manager.h"
#include "xlib_window_grab.h"

x11_window_manager::x11_window_manager() {
    setlocale(LC_ALL, ""); // see man locale
    d = open_display();
}
bool x11_window_manager::update() {
    w = get_active_window(d);
    char* windname;
    char* windclass;
    if (get_window_name(d, w, &windname) != -1) {
        active_window_name.assign(windname);

        if (get_window_class(d, w, &windclass) != -1) {
            active_window_class.assign(windclass);
            return true;
        } else {
            return false;
        }

    } else {
        return false;
    }
}