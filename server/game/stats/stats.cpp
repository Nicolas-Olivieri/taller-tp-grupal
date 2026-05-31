#include "stats.h"

#include "archetype.h"
#include "race.h"

// TODO: PLACEHOLDERS; borrar cuando podamos leerlas desde un toml dependiendo de archetype y race
#define AGILITY 10
#define INTELLIGENCE 10
#define CONSTITUTION 10
#define STRENGTH 10

Stats::Stats(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t xp_level):
        experience(current_xp_amount, xp_level),
        agility(agility_from(archetype_id, race_id)),
        constitution(constitution_from(archetype_id, race_id)),
        intelligence(intelligence_from(archetype_id, race_id)),
        strength(strength_from(archetype_id, race_id)),
        health(health_from(archetype_id, race_id)),
        mana(mana_from(archetype_id, race_id)) {}

uint8_t Stats::agility_from(uint8_t archetype_id, uint8_t race_id) {
    return AGILITY + /* PLACEHOLDERS para uso */ archetype_id + race_id + experience.get_level();
}

uint8_t Stats::constitution_from(uint8_t archetype_id, uint8_t race_id) {
    return INTELLIGENCE + /* PLACEHOLDERS para uso */ archetype_id + race_id + experience.get_level();
}

uint8_t Stats::intelligence_from(uint8_t archetype_id, uint8_t race_id) {
    return CONSTITUTION + /* PLACEHOLDERS para uso */ archetype_id + race_id + experience.get_level();
}

uint8_t Stats::strength_from(uint8_t archetype_id, uint8_t race_id) {
    return STRENGTH + /* PLACEHOLDERS para uso */ archetype_id + race_id + experience.get_level();
}

Health Stats::health_from(uint8_t archetype_id, uint8_t race_id) {
    Archetype archetype(archetype_id);
    Race race(race_id);

    return Health(race.recovery_factor, archetype.health_factor, race.health_factor, constitution,
                  experience.get_level());
}

Mana Stats::mana_from(uint8_t archetype_id, uint8_t race_id) {
    Archetype archetype(archetype_id);
    Race race(race_id);

    return Mana(race.recovery_factor, archetype.mana_factor, race.mana_factor, archetype.meditation_factor,
                intelligence, experience.get_level());
}
