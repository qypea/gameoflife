#include "x11_display.hpp"
#include "herbivore.hpp"

#include <cstdlib>

Herbivore::Herbivore(X11Display & d) : drawer_(d)
{
    health_ = healthAtStart_;
    first_ = true;
    dead_ = false;
    x_ = rand() % (drawer_.width() - size_);
    y_ = rand() % (drawer_.height() - size_);
}

void Herbivore::tick()
{
    if (dead_) {
        return;
    }

    // Use energy to live
    health_ -= healthPerTick_;
    if (health_ <= 0) {
        dead_ = true;

        // Erase self from map
        drawer_.setNext(x_, y_,
                        x_ + size_, y_ + size_,
                        X11Display::cell::empty);
        return;
    }


    // If reproducing then split health
    // Offspring will not move first tick so we won't be colliding
    if (health_ >= healthToReproduce_ && !first_) {
        health_ = healthAtStart_;
    }
    // TODO: Create offspring


    // TODO: Move
    // Decide which direction to move
    // Erase old location, path to new location

    // Update new location
    drawer_.setNext(x_, y_,
                    x_ + size_, y_ + size_,
                    X11Display::cell::herbivore);

    // No longer first tick
    first_ = false;
}
