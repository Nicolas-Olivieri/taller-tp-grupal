#include "mana.h"

#include "server/util/calculator.h"

#define FPS 30


Mana::Mana(uint8_t recovery_factor, float factor_class, float factor_race, float factor_class_meditation,
           uint8_t intelligence, uint8_t level):
        RecoverableStat(recovery_factor, factor_class, factor_race,
                        Calculator::calculate_max_mana(level, intelligence, factor_class, factor_race)),
        intelligence(intelligence),
        factor_class_meditation(factor_class_meditation) {}


void Mana::update_max(uint8_t level, uint8_t intelligence) {
    const int new_max = Calculator::calculate_max_mana(level, intelligence, factor_class, factor_race);

    const int difference = new_max - max_amount;
    max_amount = new_max;

    if (difference > 0) {
        current_amount += difference;
    }

    this->intelligence = intelligence;
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


bool Mana::loose(uint16_t amount) {
    if (current_amount <= amount) {
        // TODO: blquear el uso de hechizo, si no tiene maná suficiente, no debería poder usar el hechizo
        current_amount = 0;
    } else {
        current_amount -= amount;
    }

    return false;
}
