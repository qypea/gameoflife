#ifndef X11_DISPLAY_HPP
#define X11_DISPLAY_HPP

#include <X11/Xlib.h>
#include <vector>
#include <string>
#include <memory>

class X11Display {
    public:
        X11Display(int width, int height, const std::string & title);
        ~X11Display();

        enum class cell {
            empty,
            plant,
            herbivore,
            carnivore
        };
        void setNext(int x, int y, cell value);
        cell getCurrent(int x, int y);

        void update();

        size_t width();
        size_t height();
        void overlay(const std::string & overlay);

    private:
        Display * dsp_;
        Window win_;
        GC gc_;
        int screenNumber_;
        unsigned long white_;
        unsigned long black_;
        unsigned long green_;
        unsigned long blue_;
        unsigned long red_;

        size_t width_;
        size_t height_;
        typedef std::vector<cell> halfBacking;
        typedef std::vector<halfBacking> backing;
        std::unique_ptr<backing> next_;
        std::unique_ptr<backing> current_;

        std::string overlay_;

}; // class X11Display

#endif
