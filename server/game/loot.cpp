#include "loot.h"

Loot::Loot(uint8_t item_id): type(LootType::ITEM), item(item_id), gold(0) {}

Loot::Loot(uint16_t gold_amount): type(LootType::GOLD), item(0), gold(gold_amount) {}
