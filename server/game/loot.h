#ifndef LOOT_H
#define LOOT_H

#include <cstdint>

enum class LootType : uint8_t { ITEM, GOLD };

struct Loot {
    LootType type;
    uint8_t item;
    uint16_t gold;

    explicit Loot(uint8_t item_id);

    explicit Loot(uint16_t gold_amount);
};

#endif  // LOOT_H
