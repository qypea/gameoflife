#include "x11_display.hpp"
#include "herbivore.hpp"
#include "herd.hpp"

#include <cstdlib>

Herd::Herd(X11Display & d, int count) : drawer_(d)
{
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
            if (i == members_.end()) {
                // end, so stop
                break;
            } else {
                // Keep going through list
                i--;
                continue;
            }
        }

        // tick
        auto child = (*i)->tick();
        if (child) {
            members_.insert(child);
        }
    }
}
