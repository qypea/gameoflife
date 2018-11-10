#ifndef FPS_CLOCK_HPP
#define FPS_CLOCK_HPP

#include <sys/time.h>

class FpsClock {
public:
    FpsClock(int limit);

    void start();
    void finish();

    int fps() const;

private:
    int fps_;
    int limit_;
    int measureFrames_;

    struct timeval measureStart_;
    struct timeval tickStart_;

}; // class FpsClock

#endif // FPS_CLOCK_HPP
