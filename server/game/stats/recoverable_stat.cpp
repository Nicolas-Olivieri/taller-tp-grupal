#include "recoverable_stat.h"

#include "server/config/game_config.h"


RecoverableStat::RecoverableStat(uint8_t recovery_factor, float factor_class, float factor_race,
                                 uint16_t max):
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
    if (tick_accumulator >= GameConfig::get().get_world_constants().ticks_per_second) {
        tick_accumulator = 0;
        current_amount += recovery_factor;
        if (current_amount >= max_amount) {
            current_amount = max_amount;
        }
    }
}

void RecoverableStat::recover_all() { current_amount = max_amount; }

uint16_t RecoverableStat::get_max() const { return max_amount; }

uint16_t RecoverableStat::get_current() const { return current_amount; }

void RecoverableStat::set_current(const int amount) {
    current_amount = amount;
    if (current_amount > max_amount) {
        current_amount = max_amount;
    }
}

void RecoverableStat::recover(uint16_t amount) { set_current(current_amount + amount); }
