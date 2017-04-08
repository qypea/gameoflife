#include "gameoflife.hpp"
#include "x11_display.hpp"

#include <sys/time.h>
#include <iostream>


/* Subtract the ‘struct timeval’ values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0.
   From https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html#Elapsed-Time
   */
int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec) {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000) {
    int nsec = (x->tv_usec - y->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

int main() {
    X11Display display(600, 600);
    GameOfLife<X11Display> game(display);
    display.update();

    struct timeval tickTimer;
    struct timeval end;
    struct timeval start;
    struct timeval elapsed;
    struct timeval fps;
    fps.tv_usec = 1000000 / 15;
    fps.tv_sec = 0;
    tickTimer = fps;

    struct timeval measureStart;
    gettimeofday(&measureStart, nullptr);
    unsigned int frames = 0;

    while (true) {
        gettimeofday(&start, nullptr);

        ++frames;
        timeval_subtract(&elapsed, &start, &measureStart);
        if (elapsed.tv_sec >= 10) {
            std::cerr << "fps: " << frames / elapsed.tv_sec << std::endl;
            gettimeofday(&measureStart, nullptr);
            frames = 0;
        }

        game.tick();
        display.update();

        // Calculate remainder of tick, sleep for it
        gettimeofday(&end, nullptr);
        if (timeval_subtract(&elapsed, &end, &start) == 1) {
            continue;
        }
        if (timeval_subtract(&tickTimer, &fps, &elapsed) == 1) {
            continue;
        }
        select(1, NULL, NULL, NULL, &tickTimer);
    }

    return 0;
}
