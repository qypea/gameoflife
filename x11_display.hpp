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

        size_t width();
        size_t height();

    private:
        Display * dsp_;
        Window win_;
        Pixmap next_;
        Pixmap current_;
        GC gc_;

        size_t width_;
        size_t height_;

}; // class X11Display

#endif
