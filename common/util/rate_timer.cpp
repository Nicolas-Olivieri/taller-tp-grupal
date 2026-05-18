#include "rate_timer.h"

#include <thread>

RateTimer::RateTimer(const int fps):
        rate(1000 / fps),
        iteration(0),
        start_time(std::chrono::steady_clock::now()) {}

int RateTimer::calculate_next_iteration() {
    const auto finish_time = std::chrono::steady_clock::now();

    auto rest = rate - (finish_time - start_time);
    if (rest < std::chrono::milliseconds(0)) {
        const auto behind = -rest;
        const auto lost = behind - behind % rate;

        start_time += lost;
        iteration += lost / rate;

    } else {
        std::this_thread::sleep_for(rest);
    }

    start_time += rate;
    iteration++;

    return iteration;
}
