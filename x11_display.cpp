#include <X11/Xlib.h> // preceede most other headers!
#include <stdio.h>

#include "x11_display.hpp"

#define PIXMAP_WIDTH 1920
#define PIXMAP_HEIGHT 1080

X11Display::X11Display(int width, int height) :
        width_(width), height_(height)
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

    next_ = XCreatePixmap(dsp_, win_, PIXMAP_WIDTH, PIXMAP_HEIGHT,
                        DefaultDepth(dsp_, screenNumber));
    current_ = XCreatePixmap(dsp_, win_, PIXMAP_WIDTH, PIXMAP_HEIGHT,
                        DefaultDepth(dsp_, screenNumber));

    gc_ = XCreateGC(dsp_, win_, 0, NULL);
    XSetForeground(dsp_, gc_, black);
}

X11Display::~X11Display() {
    XFreePixmap(dsp_, next_);
    XFreePixmap(dsp_, current_);
    XDestroyWindow(dsp_, win_);
    XCloseDisplay(dsp_);
}

void X11Display::setNext(int x, int y, int value) {
    if (value == 1) {
        XDrawPoint(dsp_, next_, gc_, x, y);
    } else {
        // Noop: We cleared the pixmap
    }
}

int X11Display::getCurrent(int x, int y) {
    // TODO
    return 0;
}

void X11Display::update() {
    // Update attributes
    XWindowAttributes attr;
    XGetWindowAttributes(dsp_, win_, &attr);
    width_ = attr.width;
    height_ = attr.height;

    // Copy next to current
    XCopyArea(dsp_, next_, current_, gc_, 0, 0,
                PIXMAP_WIDTH, PIXMAP_HEIGHT, 0, 0);

    // Redraw window
    XCopyArea(dsp_, current_, win_, gc_, 0, 0, width_, height_, 0, 0);

    // Clear next
    XClearWindow(dsp_, next_);
}

size_t X11Display::width() {
    return width_;
}

size_t X11Display::height() {
    return height_;
}
