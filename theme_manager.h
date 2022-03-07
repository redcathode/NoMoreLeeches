#ifndef NML_THEMES_H
#define NML_THEMES_H
#include <FL/Fl.H>
class theme_manager {
public:
    static void loadColorScheme(int value); // 0=dark, 1=black, 2=gray, 3=tan, 4=shake
private:
    static void loadDarkColorScheme();
    static void loadBlackColorScheme();
    static void loadGrayColorScheme();
    static void loadTanColorScheme();
    static void loadShakeColorScheme();
    static void setColor(int one, int two, int three, int four);
};


#endif
