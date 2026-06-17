#include "loot.h"

#include <iostream>

#include "server/config/game_config.h"

Loot::Loot(uint8_t item_id): type(type_from_item(item_id)), item(item_id), gold(0) {}

Loot::Loot(uint16_t gold_amount): type(LootType::GOLD), item(0), gold(gold_amount) {}

LootType Loot::type_from_item(uint8_t item_id) {
    return GameConfig::get().is_secret_item(item_id) ? LootType::SECRET_ITEM : LootType::ITEM;
}
