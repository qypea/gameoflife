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

bool Herbivore::dead() const {
    return dead_;
}

int Herbivore::scoreMove(int x1, int y1, int x2, int y2) {
    int score = 0;

    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
        return -1;
    }
    if (x1 >= static_cast<int>(drawer_.width())
            || y1 >= static_cast<int>(drawer_.height())
            || x2 >= static_cast<int>(drawer_.width())
            || y2 >= static_cast<int>(drawer_.height())) {
        return -1;
    }

    auto cells = drawer_.getCurrent(x1, y1, x2, y2);
    for (auto value : cells) {
        if (value == X11Display::cell::plant) {
            // Eat it!
            ++score;
        } else if (value == X11Display::cell::empty) {
            // noop
        } else {
            // Bumped into something
            score = -1;
            break;
        }
    }
    return score;
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


    // Decide which direction to move
    int scoreNorth = scoreMove(x_, y_,
                                x_ + size_, y_ - vision_);
    int scoreEast = scoreMove(x_ + size_, y_,
                                x_ + size_ + vision_, y_ + size_);
    int scoreSouth = scoreMove(x_, y_ + size_,
                                x_ + size_, y_ + size_ + vision_);
    int scoreWest = scoreMove(x_, y_,
                                x_ - vision_, y_ + size_);
    int scoreBest = 0; // < 0 is a bad option
    scoreBest = std::max(scoreBest, scoreNorth);
    scoreBest = std::max(scoreBest, scoreEast);
    scoreBest = std::max(scoreBest, scoreSouth);
    scoreBest = std::max(scoreBest, scoreWest);

    int newX = x_;
    int newY = y_;
    if (scoreBest == scoreNorth) {
        newY = y_ - movement_;
    } else if (scoreBest == scoreEast) {
        newX = x_ + movement_;
    } else if (scoreBest == scoreSouth) {
        newY = y_ + movement_;
    } else if (scoreBest == scoreWest) {
        newX = x_ - movement_;
    } else {
        // Noop: No good options
    }

    // Erase old location, path to new location
    drawer_.setNext(x_, y_,
                    x_ + size_, y_ + size_,
                    X11Display::cell::empty);
    drawer_.setNext(x_, y_,
                    newX, newY,
                    X11Display::cell::empty);
    health_ += scoreBest;
    x_ = newX;
    y_ = newY;

    // Update new location
    drawer_.setNext(x_, y_,
                    x_ + size_, y_ + size_,
                    X11Display::cell::herbivore);

    // No longer first tick
    first_ = false;
}
