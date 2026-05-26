#include "recoverable_stat.h"

#define FPS 30


RecoverableStat::RecoverableStat(const int recovery_factor, const int factor_class, const int factor_race,
                                 const int max):
        recovery_factor(recovery_factor),
        factor_class(factor_class),
        factor_race(factor_race),
        max_amount(max),
        current_amount(max),
        tick_accumulator(0) {}


void RecoverableStat::update() {
    if (current_amount == max_amount) {
        tick_accumulator = 0;
        return;
    }

    tick_accumulator++;
    if (tick_accumulator >= FPS) {
        tick_accumulator = 0;
        current_amount += recovery_factor;
        if (current_amount >= max_amount) {
            current_amount = max_amount;
        }
    }
}
