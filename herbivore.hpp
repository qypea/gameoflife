#ifndef HERBIVORE_HPP
#define HERBIVORE_HPP

#include "x11_display.hpp"

class Herbivore
{
    public:
        Herbivore(X11Display & d);

        void tick();
        void randomize();

    private:
        X11Display & drawer_;
        int x_;
        int y_;
        int health_;
        bool first_;
        bool dead_;

        const int size_ = 5;
        const int healthAtStart_ = 50;
        const int healthToReproduce_ = 100;
        const int healthPerTick_ = 10;
        const int vision_ = 50;
        const int movement_ = size_ * 2;

}; // class herbivore

#endif // HERBIVORE_HPP
