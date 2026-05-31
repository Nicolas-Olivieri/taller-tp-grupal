#ifndef STATS_H
#define STATS_H

#include <cstdint>

#include "experience.h"
#include "health.h"
#include "mana.h"

struct Stats {
    Experience experience;

    uint8_t agility;
    uint8_t constitution;
    uint8_t intelligence;
    uint8_t strength;

    Health health;
    Mana mana;

    Stats(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t xp_level);

    uint8_t agility_from(uint8_t archetype_id, uint8_t race_id);
    uint8_t constitution_from(uint8_t archetype_id, uint8_t race_id);
    uint8_t intelligence_from(uint8_t archetype_id, uint8_t race_id);
    uint8_t strength_from(uint8_t archetype_id, uint8_t race_id);
    Health health_from(uint8_t archetype_id, uint8_t race_id);
    Mana mana_from(uint8_t archetype_id, uint8_t race_id);
};


#endif  // STATS_H
