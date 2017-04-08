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
               + drawer_.getCurrent(x - 1, y - 1)
               + drawer_.getCurrent(x - 1, y - 0)
               + drawer_.getCurrent(x - 1, y + 1)
               + drawer_.getCurrent(x - 0, y - 1)
               + drawer_.getCurrent(x - 0, y + 1)
               + drawer_.getCurrent(x + 1, y - 1)
               + drawer_.getCurrent(x + 1, y - 0)
               + drawer_.getCurrent(x + 1, y + 1);

            int current = drawer_.getCurrent(x, y);
            int tiebreaker = rand() < (RAND_MAX / 16);
            if (current == 1) { // Currently living
                if (peers < 2) { // Starved
                    drawer_.setNext(x, y, 0);
                } else if (peers > 3 + tiebreaker) { // Overcrowded
                    drawer_.setNext(x, y, 0);
                } else { // Happy
                    drawer_.setNext(x, y, 1);
                }
            } else { // Currently dead
                if (peers == 3 || peers == 6) { // Born
                    drawer_.setNext(x, y, 1);
                } else {
                    drawer_.setNext(x, y, 0);
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
            drawer_.setNext(i, j, (rand() < RAND_MAX / 4));
        }
    }
}
