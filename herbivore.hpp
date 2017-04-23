#ifndef HERBIVORE_HPP
#define HERBIVORE_HPP

#include "x11_display.hpp"

class Herbivore
{
    public:
        Herbivore(X11Display & d);
        Herbivore(X11Display & d, int x, int y);

        std::shared_ptr<Herbivore> tick();
        bool dead() const;

    private:
        X11Display & drawer_;
        int x_;
        int y_;
        int health_;
        bool first_;
        bool dead_;

        const int size_ = 5;
        const int healthAtStart_ = 50;
        const int healthToReproduce_ = 1000;
        const int healthPerTick_ = 2;
        const int vision_ = size_ * 10;
        const int movement_ = size_ * 1;


        int scoreMove(int x1, int y1, int x2, int y2) const;

}; // class herbivore

#endif // HERBIVORE_HPP
