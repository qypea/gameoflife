#include <X11/Xlib.h> // preceede most other headers!
#include <algorithm>

#include "x11_display.hpp"

X11Display::X11Display(int width, int height) :
        width_(width), height_(height),
        next_(width, std::vector<bool>(height, false)),
        current_(width, std::vector<bool>(height, false))
{
    dsp_ = XOpenDisplay(NULL);

    int screenNumber = DefaultScreen(dsp_);
    unsigned long white = WhitePixel(dsp_, screenNumber);
    unsigned long black = BlackPixel(dsp_, screenNumber);

    win_ = XCreateSimpleWindow(dsp_,
                                DefaultRootWindow(dsp_),
                                50, 50,   // origin
                                width, height, // size
                                0, black, // border
                                white);  // backgd
    XMapWindow(dsp_, win_);

    gc_ = XCreateGC(dsp_, win_, 0, NULL);
    XSetForeground(dsp_, gc_, black);
}

X11Display::~X11Display() {
    XDestroyWindow(dsp_, win_);
    XCloseDisplay(dsp_);
}

void X11Display::setNext(int x, int y, bool value) {
    next_[x][y] = value;
}

bool X11Display::getCurrent(int x, int y) {
    if (x < 0 || y < 0
            || x >= static_cast<int>(width_)
            || y >= static_cast<int>(height_)) {
        return false;
    }
    return current_[x][y];
}

void X11Display::update() {
    // Update size
    XWindowAttributes attr;
    XGetWindowAttributes(dsp_, win_, &attr);
    width_ = attr.width;
    height_ = attr.height;

    // Copy next to current
    current_ = next_;

    // Redraw window
    XClearWindow(dsp_, win_);
    for (size_t i = 0; i < std::min(next_.size(), width_); ++i) {
        for (size_t j = 0; j < std::min(next_[i].size(), height_); ++j) {
            if (next_[i][j]) {
                XDrawPoint(dsp_, win_, gc_, i, j);
            }
        }
    }
    XFlush(dsp_);

    // Clear next, resize to new size
    next_.resize(width_, std::vector<bool>(height_, false));
    current_.resize(width_, std::vector<bool>(height_));
}

size_t X11Display::width() {
    return width_;
}

size_t X11Display::height() {
    return height_;
}
