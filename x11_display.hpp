#ifndef X11_DISPLAY_HPP
#define X11_DISPLAY_HPP

#include <X11/Xlib.h>
#include <vector>
#include <string>

class X11Display {
    public:
        X11Display(int width, int height, const std::string & title);
        ~X11Display();

        void setNext(int x, int y, bool value);
        bool getCurrent(int x, int y);

        void update();

        size_t width();
        size_t height();
        void overlay(const std::string & overlay);

    private:
        Display * dsp_;
        Window win_;
        GC gc_;

        size_t width_;
        size_t height_;
        std::vector<std::vector<bool> > next_;
        std::vector<std::vector<bool> > current_;

        std::string overlay_;

}; // class X11Display

#endif
