#include "stats.h"

#include "server/TOML/statsconfig.h"

Stats::Stats(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t xp_level):
        experience(current_xp_amount, xp_level),
        agility(agility_from(archetype_id, race_id)),
        constitution(constitution_from(archetype_id, race_id)),
        intelligence(intelligence_from(archetype_id, race_id)),
        strength(strength_from(archetype_id, race_id)),
        health(health_from(archetype_id, race_id)),
        mana(mana_from(archetype_id, race_id)) {}

uint8_t Stats::agility_from(uint8_t archetype_id, uint8_t race_id) {
    uint8_t archetype_agility = StatsConfig::get().get_archetype(archetype_id).agility;
    uint8_t race_agility = StatsConfig::get().get_race(race_id).agility;

    return (archetype_agility + race_agility) / 2;
}

uint8_t Stats::constitution_from(uint8_t archetype_id, uint8_t race_id) {
    uint8_t base = StatsConfig::get().get_archetype(archetype_id).constitution_multiplier;
    uint8_t multiplier = StatsConfig::get().get_race(race_id).constitution;

    return base + experience.get_level() * multiplier;
}

uint8_t Stats::intelligence_from(uint8_t archetype_id, uint8_t race_id) {
    uint8_t base = StatsConfig::get().get_archetype(archetype_id).intelligence_multiplier;
    uint8_t multiplier = StatsConfig::get().get_race(race_id).intelligence;

    return base + experience.get_level() * multiplier;
}

uint8_t Stats::strength_from(uint8_t archetype_id, uint8_t race_id) {
    uint8_t base = StatsConfig::get().get_archetype(archetype_id).strength_multiplier;
    uint8_t multiplier = StatsConfig::get().get_race(race_id).strength;

    return base + experience.get_level() * multiplier;
}

Health Stats::health_from(uint8_t archetype_id, uint8_t race_id) {
    const ArchetypeData& archetype = StatsConfig::get().get_archetype(archetype_id);
    const RaceData& race = StatsConfig::get().get_race(race_id);

    return Health(race.recovery_factor, archetype.health_factor, race.health_factor, constitution,
                  experience.get_level());
}

Mana Stats::mana_from(uint8_t archetype_id, uint8_t race_id) {
    const ArchetypeData& archetype = StatsConfig::get().get_archetype(archetype_id);
    const RaceData& race = StatsConfig::get().get_race(race_id);

    return Mana(race.recovery_factor, archetype.mana_factor, race.mana_factor, archetype.meditation_factor,
                intelligence, experience.get_level());
}
