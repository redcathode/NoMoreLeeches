#ifndef xlib_window_grab_h
#define xlib_window_grab_h

#include <X11/Xlib.h>           // `apt-get install libx11-dev`
#include <X11/Xmu/WinUtil.h>    // `apt-get install libxmu-dev`
extern "C" {
    Display* open_display();
    Window get_active_window(Display* d);
    int get_window_name(Display* d, Window w, char** charReturn);
    int get_window_class(Display* d, Window w, char** charReturn);
}
#endif