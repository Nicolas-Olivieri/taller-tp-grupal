#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <cstdint>


class Calculator {
public:
    static uint16_t calculate_max_health(uint8_t level, uint8_t constitution, float factor_class,
                                         float factor_race);

    static uint16_t calculate_max_mana(uint8_t level, uint8_t intelligence, float factor_class,
                                       float factor_race);

    static int meditation_mana_recovery(uint8_t intelligence, float factor_class_meditation);

    static uint32_t calculate_xp_limit(uint8_t level);

    static uint32_t attack_exp(uint16_t damage, uint8_t foe_level, uint8_t own_level);

    static uint32_t kill_exp(uint16_t foe_max_health, uint8_t foe_level, uint8_t own_level);

    static uint16_t calculate_unarmed_damage(uint8_t strength);

    static uint8_t calculate_averagable_stat(uint8_t archetype_stat, uint8_t race_stat);

    static uint8_t calculate_scalable_stat(uint8_t base, uint8_t level, float multiplier);
};


#endif  // CALCULATOR_H
