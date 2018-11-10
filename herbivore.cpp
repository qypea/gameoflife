#include "herbivore.hpp"
#include "x11_display.hpp"

#include <cstdlib>

Herbivore::Herbivore(X11Display& d, int x, int y)
    : drawer_(d)
{
    health_ = healthAtStart_;
    first_ = true;
    dead_ = false;
    x_ = x;
    y_ = y;
}

Herbivore::Herbivore(X11Display& d)
    : drawer_(d)
{
    health_ = healthAtStart_;
    first_ = true;
    dead_ = false;
    x_ = rand() % (drawer_.width() - size_);
    y_ = rand() % (drawer_.height() - size_);
}

bool Herbivore::dead() const { return dead_; }

int Herbivore::scoreMove(int x1, int y1, int x2, int y2) const
{
    int score = 0;

    if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0) {
        return -1;
    }
    if (x1 >= static_cast<int>(drawer_.width()) || y1 >= static_cast<int>(drawer_.height()) || x2 >= static_cast<int>(drawer_.width()) || y2 >= static_cast<int>(drawer_.height())) {
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

std::shared_ptr<Herbivore> Herbivore::tick()
{
    std::shared_ptr<Herbivore> ret = nullptr;
    if (dead_) {
        return ret;
    }

    // Use energy to live
    health_ -= healthPerTick_;
    if (health_ <= 0) {
        dead_ = true;

        // Erase self from map
        drawer_.setNext(x_, y_, x_ + size_, y_ + size_,
            X11Display::cell::empty);
        return ret;
    }

    // Can't move or reproduce first tick
    if (first_) {
        first_ = false;
        return ret;
    }

    // If reproducing then split health
    // Offspring will not move first tick so we won't be colliding
    if (health_ >= healthToReproduce_) {
        health_ = healthAtStart_;

        ret = std::shared_ptr<Herbivore>(new Herbivore(drawer_, x_, y_));
    }

    // Decide which direction to move
    int scoreNorthMove = scoreMove(x_, y_, x_ + size_, y_ - movement_);
    int scoreNorth = scoreNorthMove * 10 + scoreMove(x_, y_, x_ + size_, y_ - vision_);

    int scoreEastMove = scoreMove(x_ + size_, y_, x_ + size_ + movement_, y_ + size_);
    int scoreEast = scoreEastMove * 10 + scoreMove(x_ + size_, y_, x_ + size_ + vision_, y_ + size_);

    int scoreSouthMove = scoreMove(x_, y_ + size_, x_ + size_, y_ + size_ + movement_);
    int scoreSouth = scoreSouthMove * 10 + scoreMove(x_, y_ + size_, x_ + size_, y_ + size_ + vision_);

    int scoreWestMove = scoreMove(x_, y_, x_ - movement_, y_ + size_);
    int scoreWest = scoreWestMove * 10 + scoreMove(x_, y_, x_ - vision_, y_ + size_);
    int scoreBest = 0; // < 0 is a bad option
    scoreBest = std::max(scoreBest, scoreNorth);
    scoreBest = std::max(scoreBest, scoreEast);
    scoreBest = std::max(scoreBest, scoreSouth);
    scoreBest = std::max(scoreBest, scoreWest);

    int newX = x_;
    int newY = y_;
    if (scoreBest <= 0) {
        // Noop: No good options
    } else if (scoreBest == scoreNorth) {
        newY = y_ - movement_;
        health_ += scoreNorthMove;
    } else if (scoreBest == scoreEast) {
        newX = x_ + movement_;
        health_ += scoreEastMove;
    } else if (scoreBest == scoreSouth) {
        newY = y_ + movement_;
        health_ += scoreSouthMove;
    } else if (scoreBest == scoreWest) {
        newX = x_ - movement_;
        health_ += scoreWestMove;
    } else {
        // I'm confused
    }

    // Erase old location, path to new location
    drawer_.setNext(x_, y_, x_ + size_, y_ + size_, X11Display::cell::empty);
    drawer_.setNext(x_, y_, newX, newY, X11Display::cell::empty);
    x_ = newX;
    y_ = newY;

    // Update new location
    drawer_.setNext(x_, y_, x_ + size_, y_ + size_,
        X11Display::cell::herbivore);

    return ret;
}
