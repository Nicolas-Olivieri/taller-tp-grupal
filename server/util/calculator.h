#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <cstdint>


class Calculator {
public:
    static int calculate_max_health(int level, int constitution, int factor_class, int factor_race);

    static int calculate_max_mana(int level, int intelligence, int factor_class, int factor_race);

    static int meditation_mana_recovery(int intelligence, int factor_class_meditation);

    static uint32_t calculate_xp_limit(int level);

    static int calculate_unarmed_damage(int strength);
};


#endif  // CALCULATOR_H
