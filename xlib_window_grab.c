//
// get the active window on X window system
//

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#include <X11/Xlib.h>           // `apt-get install libx11-dev`
#include <X11/Xmu/WinUtil.h>    // `apt-get install libxmu-dev`

Bool xerror = False;

Display* open_display(){
  // printf("connecting X server ... ");
  Display* d = XOpenDisplay(NULL);
  if(d == NULL){
    // printf("fail\n");
    exit(1);
  }else{
    // printf("success\n");
  }
  return d;
}

int handle_error(Display* display, XErrorEvent* error){
  // printf("ERROR: X11 error\n");
  xerror = True;
  return 1;
}

Window get_focus_window(Display* d){
  Window w;
  int revert_to;
  // printf("getting input focus window ... ");
  XGetInputFocus(d, &w, &revert_to); // see man
  if(xerror){
    // printf("fail\n");
    exit(1);
  }else if(w == None){
    // printf("no focus window\n");
    exit(1);
  }else{
    // printf("success (window: %d)\n", (int)w);
  }

  return w;
}

// get the top window.
// a top window have the following specifications.
//  * the start window is contained the descendent windows.
//  * the parent window is the root window.
Window get_top_window(Display* d, Window start){
  Window w = start;
  Window parent = start;
  Window root = None;
  Window *children;
  unsigned int nchildren;
  Status s;

  // printf("getting top window ... \n");
  while (parent != root) {
    w = parent;
    s = XQueryTree(d, w, &root, &parent, &children, &nchildren); // see man

    if (s)
      XFree(children);

    if(xerror){
      // printf("fail\n");
      exit(1);
    }

    // printf("  get parent (window: %d)\n", (int)w);
  }

  // printf("success (window: %d)\n", (int)w);

  return w;
}

// search a named window (that has a WM_STATE prop)
// on the descendent windows of the argment Window.
Window get_named_window(Display* d, Window start){
  Window w;
  // printf("getting named window ... ");
  w = XmuClientWindow(d, start); // see man
  if(w == start)
    // printf("fail\n");
  // printf("success (window: %d)\n", (int) w);
  return w;
}

// (XFetchName cannot get a name with multi-byte chars)
int get_window_name(Display* d, Window w, char** charReturn){
  XTextProperty prop;
  Status s;

  //// printf("window name:\n");

  s = XGetWMName(d, w, &prop); // see man
  if(!xerror && s){
    int count = 0, result;
    char **list = NULL;
    result = XmbTextPropertyToTextList(d, &prop, &list, &count); // see man
    if(result == Success){
      //// printf("\t%s\n", list[0]);
      return asprintf(charReturn, "%s", list[0]);
    }else{
      return -1;
    }
  }else{
    return -1;
  }
}

int get_window_class(Display* d, Window w, char** charReturn){
  Status s;
  XClassHint* classhint;

  // printf("application: \n");

  classhint = XAllocClassHint(); // see man
  if(xerror){
    return -1;
  }

  s = XGetClassHint(d, w, classhint); // see man
  if(xerror || s){
    return asprintf(charReturn, "%s", classhint->res_name, classhint->res_class);

  }else{
    return -1;

  }
  
}

void print_window_info(Display* d, Window w){
  // printf("--\n");
  char *name;
  
  if (get_window_name(d, w, &name) != -1) {
    // printf("%s\n", name);
  }
  char *class;
  if (get_window_class(d, w, &class) != -1) {
    // printf("%s\n", class);
  }
}

Window get_active_window(Display* d) {
  Window w;
  w = get_focus_window(d);
  w = get_top_window(d, w);
  w = get_named_window(d, w);
  return w;
}
// int main(void){
//   Display* d;
//   Window w;

//   // for XmbTextPropertyToTextList
//   setlocale(LC_ALL, ""); // see man locale

//   d = open_display();
//   XSetErrorHandler(handle_error);

//   // get active window
//   w = get_active_window(d);

//   print_window_info(d, w);
// }
