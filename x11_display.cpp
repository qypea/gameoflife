#include <X11/Xlib.h> // preceede most other headers!
#include <algorithm>

#include "x11_display.hpp"

X11Display::X11Display(int width, int height, const std::string & title) :
        width_(width), height_(height),
        next_(new backing(width, halfBacking(height))),
        current_(new backing(width, halfBacking(height)))
{
    dsp_ = XOpenDisplay(NULL);

    screenNumber_ = DefaultScreen(dsp_);
    white_ = WhitePixel(dsp_, screenNumber_);
    black_ = BlackPixel(dsp_, screenNumber_);
    green_ = 0x00FF00UL;
    blue_ = 0x0000FFUL;
    red_ = 0xFF0000UL;

    win_ = XCreateSimpleWindow(dsp_,
                                DefaultRootWindow(dsp_),
                                50, 50,   // origin
                                width, height, // size
                                0, black_, // border
                                white_);  // backgd
    XStoreName(dsp_, win_, title.c_str());
    XMapWindow(dsp_, win_);

    gc_ = XCreateGC(dsp_, win_, 0, NULL);

    XFontStruct *font_info = XLoadQueryFont(dsp_, "lucidasanstypewriter-12");
    if (font_info != nullptr) {
        XSetFont(dsp_, gc_, font_info->fid);
    }

    XSetBackground(dsp_, gc_, white_);
    XSetForeground(dsp_, gc_, black_);
}

X11Display::~X11Display() {
    XDestroyWindow(dsp_, win_);
    XCloseDisplay(dsp_);
}

void X11Display::overlay(const std::string & overlay) {
    overlay_ = overlay;
}

void X11Display::setNext(int x, int y, X11Display::cell value) {
    next_->at(x)[y] = value;
}

X11Display::cell X11Display::getCurrent(int x, int y) const {
    if (x < 0 || y < 0
            || x >= static_cast<int>(width_)
            || y >= static_cast<int>(height_)) {
        return X11Display::cell::empty;
    }
    return current_->at(x)[y];
}

void X11Display::setNext(int x1, int y1, int x2, int y2, cell value) {
    if (x2 < x1) {
        std::swap(x1, x2);
    }
    if (y2 < y1) {
        std::swap(y1, y2);
    }
    for (int i = x1; i < x2; ++i) {
        for (int j = y1; j < y2; ++j) {
            setNext(i, j, value);
        }
    }
}

std::vector<X11Display::cell>
X11Display::getCurrent(int x1, int y1, int x2, int y2) const {
    if (x2 < x1) {
        std::swap(x1, x2);
    }
    if (y2 < y1) {
        std::swap(y1, y2);
    }
    std::vector<cell> out;
    for (int i = x1; i < x2; ++i) {
        for (int j = y1; j < y2; ++j) {
            out.push_back(getCurrent(i, j));
        }
    }
    return out;
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
            if (next_->at(i)[j] == X11Display::cell::plant) {
                XSetForeground(dsp_, gc_, green_);
            } else if (next_->at(i)[j] == X11Display::cell::herbivore) {
                XSetForeground(dsp_, gc_, blue_);
            } else if (next_->at(i)[j] == X11Display::cell::carnivore) {
                XSetForeground(dsp_, gc_, red_);
            }

            if (next_->at(i)[j] != X11Display::cell::empty) {
                XDrawPoint(dsp_, win_, gc_, i, j);
            }
        }
    }

    // Draw fps overlay
    XSetForeground(dsp_, gc_, black_);
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
