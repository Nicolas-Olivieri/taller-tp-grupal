#include "item_mapper.h"

#include <algorithm>
#include <cassert>
#include <random>
#include <stdexcept>

#include "server/config/game_config.h"
#include "server/util/calculator.h"

Item ItemMapper::parse_item(uint8_t item) {
    if (is_weapon(item))
        return Weapon{item};
    if (is_armor(item))
        return Armor{item};
    if (is_helmet(item))
        return Helmet{item};
    if (is_shield(item))
        return Shield{item};
    if (is_usable(item))
        return Usable{item};

    throw std::invalid_argument("There is no item with this id");
}

bool ItemMapper::is_weapon(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.weapons_contains(item_id);
}

bool ItemMapper::is_armor(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.armors_contains(item_id);
}

bool ItemMapper::is_shield(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.shields_contains(item_id);
}

bool ItemMapper::is_helmet(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.helmets_contains(item_id);
}

bool ItemMapper::is_usable(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    return config.usables_contains(item_id);
}

TypeEffect ItemMapper::get_type_effect(uint8_t item_id) {
    uint8_t effect_type = get_item_effect_type(item_id);
    return parse_type_effect(effect_type);
}

uint8_t ItemMapper::get_item_effect_type(uint8_t item_id) {
    GameConfig& config = GameConfig::get();

    if (is_usable(item_id)) {
        const UsableItemData& data = config.get_usable(item_id);
        return data.type_effect;
    } else if (is_weapon(item_id)) {
        const WeaponData& data = config.get_weapon(item_id);
        return data.type_effect;
    }

    throw std::invalid_argument("This type of item has no special effect");
}

TypeEffect ItemMapper::parse_type_effect(uint8_t type) {
    switch (static_cast<TypeEffect>(type)) {
        case TypeEffect::HEALTH:
        case TypeEffect::MANA:
        case TypeEffect::DAMAGE:
            return static_cast<TypeEffect>(type);
        default:
            throw std::invalid_argument("This type item effect does not exist");
    }
}

uint16_t ItemMapper::get_usable_effect_amount(uint8_t item_id) {
    assert(is_usable(item_id));

    const UsableItemData& data = GameConfig::get().get_usable(item_id);

    return data.effect_amount;
}

bool ItemMapper::is_magic(const uint8_t item_id) {
    if (is_weapon(item_id))
        return GameConfig::get().get_weapon(item_id).mana_cost > 0;

    if (is_usable(item_id))
        return parse_type_effect(get_item_effect_type(item_id)) == TypeEffect::MANA;

    return false;
}
