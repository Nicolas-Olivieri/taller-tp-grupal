#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <cstdint>
#include <unordered_map>
#include <unordered_set>
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

struct WeaponData {
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

    std::unordered_set<uint8_t> armors;
    std::unordered_set<uint8_t> shields;
    std::unordered_set<uint8_t> helmets;
    std::unordered_map<uint8_t, WeaponData> weapons;
    std::unordered_map<uint8_t, UsableItemData> usables;
    std::unordered_map<uint8_t, EquipableItemData> equipables;

    std::unordered_map<uint8_t, uint16_t> item_prices;

    std::unordered_map<uint8_t, std::vector<uint8_t>> priest_items;
    std::unordered_map<uint8_t, std::vector<uint8_t>> merchant_items;

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

    const EquipableItemData& get_equipable(uint8_t id) const;

    const UsableItemData& get_usable(uint8_t id) const;

    const WeaponData& get_weapon(uint8_t id) const;

    uint16_t get_item_price(uint8_t item_id) const;

    bool usables_contains(uint8_t id) const;

    bool weapons_contains(uint8_t id) const;

    bool helmets_contains(uint8_t id) const;

    bool armors_contains(uint8_t id) const;

    bool shields_contains(uint8_t id) const;

    const std::vector<uint8_t>& get_priest_items(int id) const;

    int get_priest_max_id() const;

    const std::vector<uint8_t>& get_merchant_items(int id) const;

    int get_merchant_max_id() const;
};


#endif  // GAME_CONFIG_H
