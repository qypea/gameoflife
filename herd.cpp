#include "x11_display.hpp"
#include "herbivore.hpp"
#include "herd.hpp"

#include <cstdlib>

Herd::Herd(X11Display & d) : drawer_(d)
{
    int count = d.width() * d.height() / 20000;
    for (int i = 0; i < count; ++i) {
        members_.insert(std::shared_ptr<Herbivore>(new Herbivore(d)));
    }
}

void Herd::tick()
{
    // For each herbivore in herd
    for (auto i = members_.begin(); i != members_.end(); ++i) {
        // If dead then remove
        if ((*i)->dead()) {
            auto j = i;
            i++;
            members_.erase(j);
            i--;
            continue;
        }

        // TODO: If able to reproduce then do it

        // tick
        (*i)->tick();
    }
}
