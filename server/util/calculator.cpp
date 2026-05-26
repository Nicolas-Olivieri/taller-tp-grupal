#include "calculator.h"

#include <cmath>
#include <random>


int Calculator::calculate_max_health(const int level, const int constitution, const int factor_class,
                                     const int factor_race) {
    return constitution * factor_class * factor_race * level;
}


int Calculator::calculate_max_mana(const int level, const int intelligence, const int factor_class,
                                   const int factor_race) {
    if (factor_class == 0) {
        return 0;
    }

    return intelligence * factor_class * factor_race * level;
}


int Calculator::meditation_mana_recovery(const int intelligence, const int factor_class_meditation) {
    if (factor_class_meditation == 0) {
        return 0;
    }

    return intelligence * factor_class_meditation;
}


uint32_t Calculator::calculate_xp_limit(const int level) {
    return static_cast<uint32_t>(1000 * std::pow(level, 1.8));
}
