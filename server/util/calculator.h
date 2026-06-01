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

    static uint16_t calculate_unarmed_damage(uint8_t strength);
};


#endif  // CALCULATOR_H
