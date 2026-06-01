#ifndef STATSCONFIG_H
#define STATSCONFIG_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

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

class StatsConfig {
private:
    std::unordered_map<uint8_t, ArchetypeData> archetypes;
    std::unordered_map<uint8_t, RaceData> races;

    StatsConfig();

public:
    static StatsConfig& get();

    StatsConfig(const StatsConfig&) = delete;
    StatsConfig& operator=(const StatsConfig&) = delete;

    const ArchetypeData& get_archetype(uint8_t id) const;

    const RaceData& get_race(uint8_t id) const;
};

#endif  // STATSCONFIG_H
