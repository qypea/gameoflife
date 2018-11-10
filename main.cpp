#include "fps_clock.hpp"
#include "gameoflife.hpp"
#include "herd.hpp"
#include "x11_display.hpp"

#include <cstdlib>

int main()
{
    srand(time(nullptr));

    X11Display display(600, 600, "gameoflife");
    GameOfLife game(display);
    Herd herbs(display, 1);
    display.update();

    FpsClock fps(15);

    while (true) {
        fps.start();
        if (fps.fps() != 0) {
            display.overlay(std::to_string(fps.fps()) + " fps");
        }

        game.tick();
        herbs.tick();
        display.update();

        fps.finish();
    }

    return 0;
}
