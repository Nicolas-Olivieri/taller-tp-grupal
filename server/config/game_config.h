#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <cstdint>
#include <unordered_map>
#include <vector>


struct CooldownData {
    uint8_t attack;
    uint8_t move;
};

struct ArchetypeData {
    float health_factor;
    float mana_factor;
    float meditation_factor;
    uint8_t agility;
    float constitution_multiplier;
    float intelligence_multiplier;
    float strength_multiplier;
};

struct RaceData {
    float health_factor;
    float mana_factor;
    uint8_t recovery_factor;
    uint8_t agility;
    uint8_t constitution;
    uint8_t intelligence;
    uint8_t strength;
};

struct VariationData {
    std::vector<uint8_t> compatible_races;
    float factor;
    uint8_t agility;
    float multiplier;
};

struct UsableItemData {
    uint8_t type_effect;     // vida = 0 o mana = 1
    uint16_t effect_amount;  // 100, 200, etc.
};

struct EquipableItemData {
    uint16_t min;
    uint16_t max;
};

struct WeaponItemData {
    EquipableItemData itemData;
    uint8_t range;
    uint16_t mana_cost;  // si es 0 -> No es mágico
};


class GameConfig {
private:
    CooldownData player_cooldowns;
    CooldownData creature_cooldowns;
    std::unordered_map<uint8_t, ArchetypeData> archetypes;
    std::unordered_map<uint8_t, RaceData> races;

    std::unordered_map<uint8_t, RaceData> creatures;
    std::unordered_map<uint8_t, VariationData> variations;

    std::unordered_map<uint8_t, UsableItemData> usable_items;
    std::unordered_map<uint8_t, WeaponItemData> weapons_items;
    std::unordered_map<uint8_t, EquipableItemData> helmets_items;
    std::unordered_map<uint8_t, EquipableItemData> armors_items;
    std::unordered_map<uint8_t, EquipableItemData> shields_items;

    std::unordered_map<uint8_t, uint16_t> item_prices;

    GameConfig();

public:
    static GameConfig& get();

    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;

    const ArchetypeData& get_archetype(uint8_t id) const;

    const RaceData& get_race(uint8_t id) const;

    const RaceData& get_creature(uint8_t id) const;

    const VariationData& get_variation(uint8_t id) const;

    const CooldownData& get_player_cooldown() const;

    const CooldownData& get_creature_cooldown() const;

    const std::unordered_map<uint8_t, UsableItemData>& get_usables() const;

    const std::unordered_map<uint8_t, WeaponItemData>& get_weapons() const;

    const std::unordered_map<uint8_t, EquipableItemData>& get_helmets() const;

    const std::unordered_map<uint8_t, EquipableItemData>& get_armors() const;

    const std::unordered_map<uint8_t, EquipableItemData>& get_shields() const;

    uint16_t get_item_price(uint8_t item_id) const;
};


#endif  // GAME_CONFIG_H
