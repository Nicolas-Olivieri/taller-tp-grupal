#ifndef LOOT_H
#define LOOT_H

#include <cstdint>

#include "common/loot_type.h"

struct Loot {
    LootType type;
    uint8_t item;
    uint16_t gold;

    explicit Loot(uint8_t item_id);

    explicit Loot(uint16_t gold_amount);

    LootType type_from_item(uint8_t item_id);
};

#endif  // LOOT_H
