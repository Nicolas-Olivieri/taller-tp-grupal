#include "mana.h"

#include "server/util/calculator.h"

#define FPS 30


Mana::Mana(const int recovery_factor, const int factor_class, const int factor_race,
           const int factor_class_meditation, const int intelligence):
        RecoverableStat(recovery_factor, factor_class, factor_race,
                        Calculator::calculate_max_mana(1, intelligence, factor_class, factor_race)),
        intelligence(intelligence),
        factor_class_meditation(factor_class_meditation) {}


void Mana::update_max(const int level) {
    const int new_max = Calculator::calculate_max_mana(level, intelligence, factor_class, factor_race);

    const int difference = new_max - max_amount;
    max_amount = new_max;

    if (difference > 0) {
        current_amount += difference;
    }
}


void Mana::meditate() {
    if (current_amount == max_amount) {
        tick_accumulator = 0;
        return;
    }

    tick_accumulator++;
    if (tick_accumulator >= FPS) {
        tick_accumulator = 0;
        current_amount += Calculator::meditation_mana_recovery(intelligence, factor_class_meditation);
        if (current_amount >= max_amount) {
            current_amount = max_amount;
        }
    }
}


bool Mana::loose(const int amount) {
    current_amount -= amount;
    if (current_amount <= 0) {
        current_amount = 0;
    }

    return false;
}
