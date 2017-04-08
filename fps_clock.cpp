#include "fps_clock.hpp"

#include <sys/time.h>

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

FpsClock::FpsClock(int limit) :
        fps_(0), limit_(limit), measureFrames_(0) {
    gettimeofday(&measureStart_, nullptr);
}

void FpsClock::start() {
    gettimeofday(&tickStart_, nullptr);

    // Update fps measurement
    struct timeval tickElapsed;
    ++measureFrames_;
    timeval_subtract(&tickElapsed, &tickStart_, &measureStart_);
    if (tickElapsed.tv_sec > 1) {
        fps_ = (measureFrames_ * 1000000)
                / (tickElapsed.tv_sec * 1000000 + tickElapsed.tv_usec);
        gettimeofday(&measureStart_, nullptr);
        measureFrames_ = 0;
    }
}

void FpsClock::finish() {
    struct timeval tickRemaining;
    struct timeval tickEnd;
    struct timeval tickElapsed;
    struct timeval desiredFps;
    desiredFps.tv_usec = 1000000 / limit_;
    desiredFps.tv_sec = 0;

    // Calculate remainder of tick, sleep for it
    gettimeofday(&tickEnd, nullptr);
    if (timeval_subtract(&tickElapsed, &tickEnd, &tickStart_) == 1) {
        return;
    }
    if (timeval_subtract(&tickRemaining, &desiredFps, &tickElapsed) == 1) {
        return;
    }
    select(1, nullptr, nullptr, nullptr, &tickRemaining);
}

int FpsClock::fps() const {
    return fps_;
}
