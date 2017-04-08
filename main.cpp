#include "gameoflife.hpp"
#include "x11_display.hpp"
#include "fps_clock.hpp"

int main() {
    X11Display display(600, 600, "gameoflife");
    GameOfLife<X11Display> game(display);
    display.update();

    FpsClock fps(15);

    while (true) {
        fps.start();

        game.tick();
        display.update();

        fps.finish();
    }

    return 0;
}
