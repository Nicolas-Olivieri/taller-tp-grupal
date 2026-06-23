#include "health.h"

#include "server/util/calculator.h"

Health::Health(uint8_t recovery_factor, float factor_class, float factor_race, uint8_t constitution,
               uint8_t level):
        RecoverableStat(recovery_factor, factor_class, factor_race,
                        Calculator::calculate_max_health(level, constitution, factor_class, factor_race)) {}


void Health::update_max(uint8_t level, uint8_t constitution) {
    const int new_max = Calculator::calculate_max_health(level, constitution, factor_class, factor_race);
    max_amount = new_max;

    if (current_amount > new_max)
        current_amount = max_amount;
}


void Health::update() {
    if (current_amount == 0) {
        return;
    }

    RecoverableStat::update();
}

void Health::loose(uint16_t amount) {
    if (current_amount < amount) {
        current_amount = 0;
    } else if (current_amount > 0) {
        current_amount -= amount;
    }
}
