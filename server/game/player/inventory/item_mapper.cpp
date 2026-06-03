#include "item_mapper.h"

#include <algorithm>
#include <cassert>
#include <random>

#include "server/config/game_config.h"
#include "server/util/calculator.h"

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

bool ItemMapper::is_magic(uint8_t item_id) { return get_mana_cost(item_id) > 0; }

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

uint16_t ItemMapper::get_price(const uint8_t item_id) {
    const GameConfig& config = GameConfig::get();
    return config.get_item_price(item_id);
}

std::vector<uint8_t> ItemMapper::get_random_priest_items() {
    const GameConfig& config = GameConfig::get();
    std::vector<uint8_t> pool;

    for (const auto& [id, weapon]: config.get_weapons()) {
        if (is_magic(id)) {
            pool.push_back(id);
        }
    }

    for (const auto& [id, usable]: config.get_usables()) {
        pool.push_back(id);
    }

    // TODO: Aprovechar el método Calculator::random_number (debería ser público)
    std::random_device rd;
    std::mt19937 gen(rd());

    // TODO: Cambiar estos números mágicos por valores leídos de un TOML (mín y máx artículos a la venta)
    std::uniform_int_distribution<size_t> dis(3, 6);

    const size_t amount_to_pick = std::min(dis(gen), pool.size());

    std::vector<uint8_t> selected_items;
    std::ranges::sample(pool, std::back_inserter(selected_items), amount_to_pick, gen);

    return selected_items;
}
