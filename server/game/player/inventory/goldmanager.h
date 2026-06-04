#ifndef GOLDMANAGER_H
#define GOLDMANAGER_H

#include <cstdint>
#include <stdexcept>

struct NotEnoughGold: public std::runtime_error {
    NotEnoughGold(): std::runtime_error("The player does not have enough money to buy this.") {}
};

class GoldManager {
private:
    uint16_t current_gold;
    uint16_t max_safe_gold;
    uint16_t max_total_gold;

public:
    GoldManager(uint16_t gold_amount, uint8_t level);

    void update_max(uint8_t level);

    void add(uint16_t amount);

    void spend(uint16_t amount);

    uint16_t get_safe_gold() const;

    uint16_t get_excess_gold() const;
};

#endif  // GOLDMANAGER_H
