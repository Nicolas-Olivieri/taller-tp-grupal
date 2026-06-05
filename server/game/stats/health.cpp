#include "health.h"

#include "server/util/calculator.h"

Health::Health(uint8_t recovery_factor, float factor_class, float factor_race, uint8_t constitution,
               uint8_t level):
        RecoverableStat(recovery_factor, factor_class, factor_race,
                        Calculator::calculate_max_health(level, constitution, factor_class, factor_race)),
        constitution(constitution) {}


void Health::update_max(uint8_t level, uint8_t constitution) {
    const int new_max = Calculator::calculate_max_health(level, constitution, factor_class, factor_race);

    const int difference = new_max - max_amount;
    max_amount = new_max;

    if (difference > 0) {
        current_amount += difference;
    }

    this->constitution = constitution;
}


void Health::update() {
    if (current_amount == 0) {
        return;
    }

    RecoverableStat::update();
}

bool Health::loose(uint16_t amount) {
    if (current_amount == 0) {
        return false;
    } else if (current_amount < amount) {
        // TODO: Morir
        current_amount = 0;
        return true;
    }

    current_amount -= amount;
    return false;
}
