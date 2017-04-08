#include <X11/Xlib.h> // preceede most other headers!
#include <algorithm>

#include "x11_display.hpp"

X11Display::X11Display(int width, int height, const std::string & title) :
        width_(width), height_(height),
        next_(new backing(width, halfBacking(height, false))),
        current_(new backing(width, halfBacking(height, false)))
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
    XStoreName(dsp_, win_, title.c_str());
    XMapWindow(dsp_, win_);

    gc_ = XCreateGC(dsp_, win_, 0, NULL);

    XFontStruct *font_info = XLoadQueryFont(dsp_, "lucidasanstypewriter-12");
    if (font_info != nullptr) {
        XSetFont(dsp_, gc_, font_info->fid);
    }

    XSetBackground(dsp_, gc_, white);
    XSetForeground(dsp_, gc_, black);
}

X11Display::~X11Display() {
    XDestroyWindow(dsp_, win_);
    XCloseDisplay(dsp_);
}

void X11Display::overlay(const std::string & overlay) {
    overlay_ = overlay;
}

void X11Display::setNext(int x, int y, bool value) {
    next_->at(x)[y] = value;
}

bool X11Display::getCurrent(int x, int y) {
    if (x < 0 || y < 0
            || x >= static_cast<int>(width_)
            || y >= static_cast<int>(height_)) {
        return false;
    }
    return current_->at(x)[y];
}

void X11Display::update() {
    // Update size
    XWindowAttributes attr;
    XGetWindowAttributes(dsp_, win_, &attr);

    if (static_cast<size_t>(attr.width) != width_
            || static_cast<size_t>(attr.height) != height_) {
        width_ = attr.width;
        height_ = attr.height;

        next_->resize(width_);
        current_->resize(width_);
        for (size_t x = 0; x < width_; ++x) {
            next_->at(x).resize(height_);
            current_->at(x).resize(height_);
        }
    }

    // Redraw window
    XClearWindow(dsp_, win_);
    for (size_t i = 0; i < std::min(next_->size(), width_); ++i) {
        for (size_t j = 0; j < std::min(next_->at(i).size(), height_); ++j) {
            if (next_->at(i)[j]) {
                XDrawPoint(dsp_, win_, gc_, i, j);
            }
        }
    }

    // Draw fps overlay
    XDrawImageString(dsp_, win_, gc_, 10, 20,
                     overlay_.c_str(), overlay_.size());

    XFlush(dsp_);

    // Swap buffers
    std::swap(next_, current_);
}

size_t X11Display::width() {
    return width_;
}

size_t X11Display::height() {
    return height_;
}
