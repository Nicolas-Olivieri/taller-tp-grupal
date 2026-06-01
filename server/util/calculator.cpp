#include "calculator.h"

#include <cmath>
#include <random>


uint16_t Calculator::calculate_max_health(uint8_t level, uint8_t constitution, float factor_class,
                                          float factor_race) {
    return constitution * factor_class * factor_race * level;
}


uint16_t Calculator::calculate_max_mana(uint8_t level, uint8_t intelligence, float factor_class,
                                        float factor_race) {
    if (factor_class == 0) {
        return 0;
    }

    return intelligence * factor_class * factor_race * level;
}


int Calculator::meditation_mana_recovery(uint8_t intelligence, float factor_class_meditation) {
    if (factor_class_meditation == 0) {
        return 0;
    }

    return intelligence * factor_class_meditation;
}


uint32_t Calculator::calculate_xp_limit(uint8_t level) {
    return static_cast<uint32_t>(1000 * std::pow(level, 1.8));
}


// TODO: Refactorizar el generador de números aleatorios para no crearlo en cada golpe
uint16_t Calculator::calculate_unarmed_damage(uint8_t strength) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 3);

    return strength * dis(gen);
}
