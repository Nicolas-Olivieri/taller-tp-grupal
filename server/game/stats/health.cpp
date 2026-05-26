#include "health.h"

#include "server/util/calculator.h"


Health::Health(const int recovery_factor, const int factor_class, const int factor_race,
               const int constitution):
        RecoverableStat(recovery_factor, factor_class, factor_race,
                        Calculator::calculate_max_health(1, constitution, factor_class, factor_race)),
        constitution(constitution) {}


void Health::update_max(const int level) {
    const int new_max = Calculator::calculate_max_health(level, constitution, factor_class, factor_race);

    const int difference = new_max - max_amount;
    max_amount = new_max;

    if (difference > 0) {
        current_amount += difference;
    }
}


void Health::update() {
    if (current_amount == 0) {
        return;
    }

    RecoverableStat::update();
}


bool Health::loose(const int amount) {
    if (current_amount == 0) {
        return false;
    }

    current_amount -= amount;
    if (current_amount <= 0) {
        // TODO: Morir
        current_amount = 0;
        return true;
    }

    return false;
}


int Health::get_current() const { return current_amount; }
