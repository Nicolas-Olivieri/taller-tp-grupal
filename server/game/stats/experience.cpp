#include "experience.h"

#include "server/util/calculator.h"


Experience::Experience(uint32_t current_amount, uint8_t level):
        current_amount(current_amount), level(level), limit(Calculator::calculate_xp_limit(level)) {}

uint8_t Experience::get_level() const { return level; }


uint32_t Experience::get_current_amount() const { return current_amount; }


bool Experience::earn_xp(uint32_t amount) {
    current_amount += amount;
    bool leveled_up = false;

    while (current_amount >= limit) {
        current_amount -= limit;
        level++;
        leveled_up = true;
        limit = Calculator::calculate_xp_limit(level);
    }

    return leveled_up;
}

bool Experience::loose_xp(uint32_t amount) {
    if (current_amount < amount) {
        uint32_t difference = amount - current_amount;
        set_level(level - 1);
        current_amount = limit - difference;
        return true;
    }

    current_amount -= amount;
    return false;
}


void Experience::set_level(uint8_t new_level) {
    current_amount = 0;
    level = new_level;
    limit = Calculator::calculate_xp_limit(level);
}
