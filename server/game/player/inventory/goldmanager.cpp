#include "goldmanager.h"

#include <algorithm>

#include "server/util/calculator.h"

GoldManager::GoldManager(uint16_t gold_amount, uint8_t level): current_gold(0) {
    update_max(level);
    add(gold_amount);
}

void GoldManager::update_max(uint8_t level) {
    max_safe_gold = Calculator::calculate_max_gold(level);
    max_total_gold = max_safe_gold + Calculator::calculate_max_excess_gold(max_safe_gold);

    if (current_gold > max_total_gold)
        current_gold = max_total_gold;
}

void GoldManager::add(uint16_t amount) {
    // TODO: Considerar el caso de monedero lleno al intentar retirar oro del banco
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
