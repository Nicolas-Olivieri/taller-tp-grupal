#ifndef STATS_H
#define STATS_H

#include <cstdint>

#include "server/TOML/statsconfig.h"

#include "experience.h"
#include "health.h"
#include "mana.h"

struct Stats {
    uint8_t archetype_id;
    uint8_t race_id;

    Experience experience;

    uint8_t agility;
    uint8_t constitution;
    uint8_t intelligence;
    uint8_t strength;

    Health health;
    Mana mana;

    Stats(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t xp_level);

    const ArchetypeData& archetype();

    const RaceData& race();

    void upgrade();
};


#endif  // STATS_H
