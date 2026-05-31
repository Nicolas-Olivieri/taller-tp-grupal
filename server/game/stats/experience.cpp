#include "experience.h"

#include "server/util/calculator.h"


Experience::Experience(uint32_t current_amount, uint8_t level):
        current_amount(current_amount), level(level) {}

uint8_t Experience::get_level() const { return level; }


uint32_t Experience::get_current_amount() const { return current_amount; }


void Experience::earn_xp(const uint32_t amount) {
    current_amount += amount;
    bool leveled_up = false;

    uint32_t limit = Calculator::calculate_xp_limit(level);
    while (current_amount >= limit) {
        current_amount -= limit;
        level++;
        leveled_up = true;
        limit = Calculator::calculate_xp_limit(level);
    }

    if (leveled_up) {
        // TODO: Notificar para que Health y Mana invoquen a update_max()
    }
}
