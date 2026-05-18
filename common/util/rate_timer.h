#ifndef RATE_TIMER_H
#define RATE_TIMER_H

#include <chrono>

class RateTimer {
private:
    std::chrono::milliseconds rate;
    int iteration;
    std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
    explicit RateTimer(int fps);

    int calculate_next_iteration();
};


#endif
