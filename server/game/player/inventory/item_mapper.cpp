#include "item_mapper.h"

#include <cassert>

#include "server/config/game_config.h"

bool ItemMapper::is_equipable(uint8_t item_id) {
    return is_weapon(item_id) or is_armor(item_id) or is_shield(item_id) or is_helmet(item_id);
}

bool ItemMapper::is_weapon(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.get_weapons().contains(item_id);
}

bool ItemMapper::is_armor(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.get_armors().contains(item_id);
}

bool ItemMapper::is_shield(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.get_shields().contains(item_id);
}

bool ItemMapper::is_helmet(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.get_helmets().contains(item_id);
}

bool ItemMapper::is_usable(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.get_usables().contains(item_id);
}

int ItemMapper::get_max(uint8_t item_id) {
    assert(is_equipable(item_id));
    GameConfig& config = GameConfig::get();

    if (is_weapon(item_id))
        return config.get_weapons().find(item_id)->second.itemData.max;
    if (is_shield(item_id))
        return config.get_shields().find(item_id)->second.max;
    if (is_helmet(item_id))
        return config.get_helmets().find(item_id)->second.max;

    assert(is_armor(item_id));
    return config.get_armors().find(item_id)->second.max;
}

int ItemMapper::get_min(uint8_t item_id) {
    assert(is_equipable(item_id));
    GameConfig& config = GameConfig::get();

    if (is_weapon(item_id))
        return config.get_weapons().find(item_id)->second.itemData.min;
    if (is_shield(item_id))
        return config.get_shields().find(item_id)->second.min;
    if (is_helmet(item_id))
        return config.get_helmets().find(item_id)->second.min;

    assert(is_armor(item_id));
    return config.get_armors().find(item_id)->second.min;
}

int ItemMapper::get_range(uint8_t item_id) {
    assert(is_weapon(item_id));
    GameConfig& config = GameConfig::get();

    return config.get_weapons().find(item_id)->second.range;
}

int ItemMapper::get_mana_cost(uint8_t item_id) {
    assert(is_weapon(item_id));
    GameConfig& config = GameConfig::get();

    return config.get_weapons().find(item_id)->second.mana_cost;
}

bool ItemMapper::is_magic(uint8_t item_id) { return get_mana_cost(item_id) == 0; }

uint8_t ItemMapper::get_usable_type_effect(uint8_t item_id) {
    assert(is_usable(item_id));
    GameConfig& config = GameConfig::get();

    return config.get_usables().find(item_id)->second.type_effect;
}

uint16_t ItemMapper::get_usable_effect_amount(uint8_t item_id) {
    assert(is_usable(item_id));
    GameConfig& config = GameConfig::get();

    return config.get_usables().find(item_id)->second.effect_amount;
}
