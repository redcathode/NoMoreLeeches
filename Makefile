# We don't know what compiler to use to build fltk on this machine - but fltk-config does...
CC  = $(shell fltk-config --cc)
CXX = $(shell fltk-config --cxx)

# Set the flags for compiler: fltk-config knows the basic settings, then we can add our own...


# We don't know what libraries to link with: fltk-config does...
LINKFLTK = $(shell fltk-config --ldstaticflags) $(shell pkg-config --libs x11 xmu) -lstdc++fs
LINKFLTK_GL = $(shell fltk-config --use-gl --ldstaticflags)
LINKFLTK_IMG = $(shell fltk-config --use-images --ldstaticflags)

# Possible steps to run after linking...
STRIP      = strip
POSTBUILD  = fltk-config --post # Required on OSX, does nothing on other platforms, so safe to call
ifeq ($(OS),Windows_NT)
MAIN_DEPS = main.cxx main_window.h win32_window_manager.h blocked_window.h theme_manager.h
EXE_OBJ_DEPS = main.o main_window.o win32_window_manager.o blocked_window.o theme_manager.o
CFLAGS   = $(shell fltk-config --cflags) -Wall
CXXFLAGS = $(shell fltk-config --cxxflags) -Wall -lstdc++fs
EXENAME = NoMoreLeeches.exe
else
MAIN_DEPS = main.cxx main_window.h x11_window_manager.h blocked_window.h theme_manager.h
EXE_OBJ_DEPS = main.o xlib_window_grab.o main_window.o x11_window_manager.o blocked_window.o theme_manager.o
CFLAGS   = $(shell fltk-config --cflags) $(shell pkg-config --libs x11 xmu) -Wall
CXXFLAGS = $(shell fltk-config --cxxflags) $(shell pkg-config --libs x11 xmu) -Wall -lstdc++fs
EXENAME = NoMoreLeeches
endif

# Define what your target application is called
all: NoMoreLeeches

# Define how to build the various object files...
theme_manager.o: theme_manager.cxx theme_manager.h
		$(CXX) -c $< $(CXXFLAGS)
xlib_window_grab.o: xlib_window_grab.c xlib_window_grab.h  # a "plain" C file
		$(CC) -c $< $(CCFLAGS)
x11_window_manager.o: x11_window_manager.cxx x11_window_manager.h xlib_window_grab.h
		$(CXX) -c $< $(CXXFLAGS)

win32_window_manager.o: win32_window_manager.cxx win32_window_manager.h
		$(CXX) -c $< $(CXXFLAGS)

main_window.o: main_window.cxx main_window.h  # a C++ file
		$(CXX) -c $< $(CXXFLAGS)

blocked_window.o: blocked_window.cxx blocked_window.h
		$(CXX) -c $< $(CXXFLAGS)

main.o: $(MAIN_DEPS)
		$(CXX) -c $< $(CXXFLAGS)


NoMoreLeeches:  $(EXE_OBJ_DEPS)
		$(CXX) -o $@ $(EXE_OBJ_DEPS) $(LINKFLTK)
		$(STRIP) ./$(EXENAME)

clean: 
	rm *.o $(EXENAME)

run: NoMoreLeeches
	./$(EXENAME)
