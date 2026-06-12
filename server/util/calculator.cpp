#include "calculator.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>

#include "server/game/items/item_mapper.h"

uint16_t Calculator::calculate_max_health(uint8_t level, uint8_t constitution, float factor_class,
                                          float factor_race) {
    return constitution * factor_class * factor_race * level;
}


uint16_t Calculator::calculate_max_mana(uint8_t level, uint8_t intelligence, float factor_class,
                                        float factor_race) {
    if (factor_class == 0) {
        return 0;
    }

    return intelligence * factor_class * factor_race * level;
}

uint16_t Calculator::calculate_max_gold(uint8_t level) {
    return static_cast<uint16_t>(100 * std::pow(level, 1.1));
}

uint16_t Calculator::calculate_max_excess_gold(uint16_t max_safe_gold) { return max_safe_gold / 2; }

int Calculator::meditation_mana_recovery(uint8_t intelligence, float factor_class_meditation) {
    if (factor_class_meditation == 0) {
        return 0;
    }

    return intelligence * factor_class_meditation;
}


uint32_t Calculator::calculate_xp_limit(uint8_t level) {
    return static_cast<uint32_t>(1000 * std::pow(level, 1.8));
}

uint32_t Calculator::attack_exp(uint16_t damage, uint8_t foe_level, uint8_t own_level) {
    return damage * std::max(foe_level - own_level + 10, 0);
}

uint32_t Calculator::kill_exp(uint16_t foe_max_health, uint8_t foe_level, uint8_t own_level) {
    return random_float(0.0f, 0.1f) * foe_max_health * std::max(foe_level - own_level + 10, 0);
}

int Calculator::random_number(const int min, const int max) {
    assert(min <= max);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

uint8_t Calculator::random_number(const uint8_t min, const uint8_t max) {
    assert(min <= max);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(min, max);
    return dis(gen);
}

int Calculator::random_from_weighted_probabilities(const std::vector<float>& probabilities) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::discrete_distribution<int> dis(probabilities.begin(), probabilities.end());

    return dis(gen);
}

uint16_t Calculator::calculate_damage(const uint8_t strength, const Equipment& equipment) {
    return strength * get_random_from_item(equipment.weapon);
}

uint16_t Calculator::calculate_defense(const Equipment& equipment, const float& buff_factor) {
    const int helmet_defense = equipment.helmet == NO_ITEM ? 0 : get_random_from_item(equipment.helmet);
    const int armor_defense = equipment.armor == NO_ITEM ? 0 : get_random_from_item(equipment.armor);
    const int shield_defense = equipment.shield == NO_ITEM ? 0 : get_random_from_item(equipment.shield);

    return (helmet_defense + armor_defense + shield_defense) * buff_factor;
}

int Calculator::get_random_from_item(const uint8_t item) {
    const EquipableItemData data = GameConfig::get().get_equipable(item);

    return random_number(data.min, data.max);
}

float Calculator::random_float(const float min, const float max) {
    assert(min <= max);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);

    return dis(gen);
}

uint8_t Calculator::calculate_averagable_stat(uint8_t archetype_stat, uint8_t race_stat) {
    return std::min(UINT8_MAX, (archetype_stat + race_stat) / 2);
}

uint8_t Calculator::calculate_scalable_stat(uint8_t base, uint8_t level, float multiplier) {
    return std::min(UINT8_MAX, base + static_cast<uint8_t>(level * multiplier));
}

bool Calculator::can_dodge(const int agility) { return std::pow(random_float(0.0f, 1.0f), agility) < 0.001f; }

uint16_t Calculator::calculate_random_drop_gold(uint8_t max_healh) {
    return random_float(0.1, 0.2) * max_healh;
}

uint16_t Calculator::calculate_damage(const uint8_t strength, const Equipment& equipment, float buff_factor) {
    return calculate_damage(strength, equipment) * buff_factor;
}
