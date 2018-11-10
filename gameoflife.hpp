#ifndef GAMEOFLIFE_HPP
#define GAMEOFLIFE_HPP

#include "x11_display.hpp"

class GameOfLife {
public:
    GameOfLife(X11Display& d);

    void tick();
    void randomize();

private:
    X11Display& drawer_;

}; // class gameoflife

#endif // GAMEOFLIFE_HPP
