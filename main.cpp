#include "gameoflife.hpp"
#include "x11_display.hpp"
#include "fps_clock.hpp"

int main() {
    X11Display display(600, 600, "gameoflife");
    GameOfLife game(display);
    display.update();

    FpsClock fps(15);

    while (true) {
        fps.start();
        if (fps.fps() != 0) {
            display.overlay(std::to_string(fps.fps()) + " fps");
        }

        game.tick();
        display.update();

        fps.finish();
    }

    return 0;
}
