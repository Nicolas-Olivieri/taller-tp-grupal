#include "goldmanager.h"

#include <algorithm>

#include "server/util/calculator.h"

GoldManager::GoldManager(uint16_t gold_amount, uint8_t level) {
    update_max(level);
    add(gold_amount);
}

void GoldManager::update_max(uint8_t level) {
    max_safe_gold = Calculator::calculate_max_gold(level);
    max_total_gold = max_safe_gold + Calculator::calculate_max_excess_gold(max_safe_gold);
}

void GoldManager::add(uint16_t amount) {
    if (max_total_gold - current_gold < amount) {
        current_gold = max_total_gold;
    } else {
        current_gold += amount;
    }
}

void GoldManager::spend(uint16_t amount) {
    if (amount > current_gold)
        throw NotEnoughGold();

    current_gold -= amount;
}

uint16_t GoldManager::get_safe_gold() const { return std::min(max_safe_gold, current_gold); }

uint16_t GoldManager::get_excess_gold() const {
    return current_gold > max_safe_gold ? current_gold - max_safe_gold : 0;
}
