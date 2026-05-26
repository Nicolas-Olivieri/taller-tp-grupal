#ifndef STATS_H
#define STATS_H

#include <cstdint>

#include "archetype.h"
#include "experience.h"
#include "health.h"
#include "mana.h"
#include "race.h"


struct Stats {
    uint8_t agility;
    uint8_t constitution;
    uint8_t intelligence;
    uint8_t strength;

    Experience experience;
    Health health;
    Mana mana;

    explicit Stats(uint8_t agility, uint8_t constitution, uint8_t intelligence, uint8_t strength,
                   Archetype archetype, Race race);
};


#endif  // STATS_H
