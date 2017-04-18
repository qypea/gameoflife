#ifndef HERD_HPP
#define HERD_HPP

#include "x11_display.hpp"
#include "herbivore.hpp"

#include <set>

class Herd
{
    public:
        Herd(X11Display & d, int count);

        void tick();

    private:
        X11Display & drawer_;

        std::set<std::shared_ptr<Herbivore> > members_;

}; // class herd

#endif // HERD_HPP
