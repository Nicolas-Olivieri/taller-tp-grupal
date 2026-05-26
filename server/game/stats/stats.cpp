#include "stats.h"


Stats::Stats(const uint8_t agility, const uint8_t constitution, const uint8_t intelligence,
             const uint8_t strength, const Archetype archetype, const Race race):
        agility(agility),
        constitution(constitution),
        intelligence(intelligence),
        strength(strength),
        health(race.recovery_factor, archetype.health_factor, race.health_factor, constitution),
        mana(race.recovery_factor, archetype.mana_factor, race.mana_factor, archetype.meditation_factor,
             intelligence) {}
