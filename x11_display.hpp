#ifndef X11_DISPLAY_HPP
#define X11_DISPLAY_HPP

#include <X11/Xlib.h>

class X11Display {
    public:
        X11Display(int width, int height);
        ~X11Display();

        void setNext(int x, int y, int value);
        int getCurrent(int x, int y);

        void update();

        int width();
        int height();

    private:
        Display * dsp_;
        Window win_;
        Pixmap next_;
        Pixmap current_;
        GC gc_;

        int width_;
        int height_;

}; // class X11Display

#endif
