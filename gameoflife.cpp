#include "x11_display.hpp"
#include "gameoflife.hpp"

#include <cstdlib>

GameOfLife::GameOfLife(X11Display & d) : drawer_(d)
{
    randomize();
}

void GameOfLife::tick()
{
    int width = drawer_.width();
    int height = drawer_.height();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int peers = 0
               + (drawer_.getCurrent(x - 1, y - 1) == X11Display::cell::plant)
               + (drawer_.getCurrent(x - 1, y - 0) == X11Display::cell::plant)
               + (drawer_.getCurrent(x - 1, y + 1) == X11Display::cell::plant)
               + (drawer_.getCurrent(x - 0, y - 1) == X11Display::cell::plant)
               + (drawer_.getCurrent(x - 0, y + 1) == X11Display::cell::plant)
               + (drawer_.getCurrent(x + 1, y - 1) == X11Display::cell::plant)
               + (drawer_.getCurrent(x + 1, y - 0) == X11Display::cell::plant)
               + (drawer_.getCurrent(x + 1, y + 1) == X11Display::cell::plant);

            X11Display::cell current = drawer_.getCurrent(x, y);
            int tiebreaker = rand() < (RAND_MAX / 32);
            if (current == X11Display::cell::plant) { // Currently living
                if (peers < 2) { // Starved
                    drawer_.setNext(x, y, X11Display::cell::empty);
                } else if (peers > 3 + tiebreaker) { // Overcrowded
                    drawer_.setNext(x, y, X11Display::cell::empty);
                } else { // Happy
                    drawer_.setNext(x, y, X11Display::cell::plant);
                }
            } else if (current == X11Display::cell::empty) { // Currently empty
                if (peers == 3 || peers == 6) { // Born
                    drawer_.setNext(x, y, X11Display::cell::plant);
                } else {
                    drawer_.setNext(x, y, X11Display::cell::empty);
                }
            }
        }
    }
}

void GameOfLife::randomize() {
    int width = drawer_.width();
    int height = drawer_.height();
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if (rand() < RAND_MAX / 4) {
                drawer_.setNext(i, j, X11Display::cell::plant);
            } else {
                drawer_.setNext(i, j, X11Display::cell::empty);
            }
        }
    }
}
