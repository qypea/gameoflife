#include "gameoflife.hpp"
#include "x11_display.hpp"

int main() {
    X11Display display(200, 200);
    GameOfLife<X11Display> game(display);
    display.update();

    struct timeval tickTimer;
    struct timeval fps;
    fps.tv_usec = 1000000 / 30;
    fps.tv_sec = 0;
    tickTimer = fps;

    while (true) {
        select(1, NULL, NULL, NULL, &tickTimer);
        tickTimer = fps;

        game.tick();
        display.update();
    }

    return 0;
}
