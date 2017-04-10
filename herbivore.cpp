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
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                drawer_.setNext(x_ + i, y_ + j, X11Display::cell::empty);
            }
        }
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
    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j) {
            drawer_.setNext(x_ + i, y_ + j, X11Display::cell::herbivore);
        }
    }

    // No longer first tick
    first_ = false;
}
