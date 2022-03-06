# We don't know what compiler to use to build fltk on this machine - but fltk-config does...
CC  = $(shell fltk-config --cc)
CXX = $(shell fltk-config --cxx)

# Set the flags for compiler: fltk-config knows the basic settings, then we can add our own...
CFLAGS   = $(shell fltk-config --cflags) $(shell pkg-config --libs x11 xmu) -Wall #-I/other/include/paths...
CXXFLAGS = $(shell fltk-config --cxxflags) $(shell pkg-config --libs x11 xmu) -Wall -lstdc++fs #-I/other/include/paths...

# We don't know what libraries to link with: fltk-config does...
LINKFLTK = $(shell fltk-config --ldstaticflags) $(shell pkg-config --libs x11 xmu) -lstdc++fs
LINKFLTK_GL = $(shell fltk-config --use-gl --ldstaticflags)
LINKFLTK_IMG = $(shell fltk-config --use-images --ldstaticflags)

# Possible steps to run after linking...
STRIP      = strip
POSTBUILD  = fltk-config --post # Required on OSX, does nothing on other platforms, so safe to call


# Define what your target application is called
all: NoMoreLeeches

# Define how to build the various object files...

xlib_window_grab.o: xlib_window_grab.c xlib_window_grab.h  # a "plain" C file
		$(CC) -c $< $(CCFLAGS)
x11_window_manager.o: x11_window_manager.cxx x11_window_manager.h xlib_window_grab.h
		$(CXX) -c $< $(CXXFLAGS)

main_window.o: main_window.cxx main_window.h xlib_window_grab.h  # a C++ file
		$(CXX) -c $< $(CXXFLAGS)

blocked_window.o: blocked_window.cxx blocked_window.h
		$(CXX) -c $< $(CXXFLAGS)

main.o: main.cxx main_window.h xlib_window_grab.h blocked_window.h
		$(CXX) -c $< $(CXXFLAGS)


# Now define how to link the final app - let's assume it needs image and OpenGL support
NoMoreLeeches:  main.o xlib_window_grab.o main_window.o x11_window_manager.o blocked_window.o
		$(CXX) -o $@ main.o xlib_window_grab.o main_window.o x11_window_manager.o blocked_window.o $(LINKFLTK)
		$(STRIP) $@
		$(POSTBUILD) $@  # only required on OSX, but call it anyway for portability

clean: 
	rm *.o NoMoreLeeches

run: NoMoreLeeches
	./NoMoreLeeches
