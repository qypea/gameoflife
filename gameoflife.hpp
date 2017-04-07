#ifndef GAMEOFLIFE_HPP
#define GAMEOFLIFE_HPP

template <typename Draw>
class GameOfLife
{
    public:
        GameOfLife(Draw & d) :
            drawer_(d)
        {
        }

        void tick()
        {
            int width = drawer_.width();
            int height = drawer_.height();

            for (int x = 0; x < width; ++x) {
                for (int y = 0; y < height; ++y) {
                    int peers = 0
                       + drawer_.getCurrent(x - 1, y - 1)
                       + drawer_.getCurrent(x - 1, y - 0)
                       + drawer_.getCurrent(x - 1, y + 1)
                       + drawer_.getCurrent(x - 0, y - 1)
                       + drawer_.getCurrent(x - 0, y + 1)
                       + drawer_.getCurrent(x + 1, y - 1)
                       + drawer_.getCurrent(x + 1, y - 0)
                       + drawer_.getCurrent(x + 1, y + 1);

                    int current = drawer_.getCurrent(x, y);
                    if (current == 1) { // Currently living
                        if (peers < 2) { // Starved
                            drawer_.setNext(x, y, 0);
                        } else if (peers > 3) { // Overcrowded
                            drawer_.setNext(x, y, 0);
                        } else { // Happy
                            drawer_.setNext(x, y, 1);
                        }
                    } else { // Currently dead
                        if (peers == 3) { // Born
                            drawer_.setNext(x, y, 1);
                        } else {
                            drawer_.setNext(x, y, 0);
                        }
                    }
                }
            }
        }

    private:
        Draw & drawer_;

}; // class gameoflife

#endif // GAMEOFLIFE_HPP
