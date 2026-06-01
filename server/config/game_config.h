#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <cstdint>
#include <unordered_map>


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


class GameConfig {
private:
    CooldownData cooldowns{};
    std::unordered_map<uint8_t, ArchetypeData> archetypes;
    std::unordered_map<uint8_t, RaceData> races;

    GameConfig();

public:
    static GameConfig& get();

    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;

    const ArchetypeData& get_archetype(uint8_t id) const;

    const RaceData& get_race(uint8_t id) const;

    const CooldownData& get_cooldown() const;
};


#endif  // GAME_CONFIG_H
