#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <cstdint>
#include <random>
#include <vector>

#include "server/game/items/equipment.h"


class Calculator {
public:
    static uint16_t calculate_max_health(uint8_t level, uint8_t constitution, float factor_class,
                                         float factor_race);

    static uint16_t calculate_max_mana(uint8_t level, uint8_t intelligence, float factor_class,
                                       float factor_race);

    static uint16_t calculate_max_gold(uint8_t level);

    static uint16_t calculate_max_excess_gold(uint16_t max_safe_gold);

    static int meditation_mana_recovery(uint8_t intelligence, float factor_class_meditation);

    static uint32_t calculate_xp_limit(uint8_t level);

    static uint32_t attack_exp(uint16_t damage, uint8_t foe_level, uint8_t own_level);

    static uint32_t kill_exp(uint16_t foe_max_health, uint8_t foe_level, uint8_t own_level);

    static uint16_t calculate_damage(const uint8_t strength, const Equipment& equipment);

    static uint16_t calculate_defense(const Equipment& equipment);

    static bool can_dodge(const int agility);

    static uint16_t calculate_random_drop_gold(uint8_t max_healh);

    static uint8_t calculate_averagable_stat(uint8_t archetype_stat, uint8_t race_stat);

    static uint8_t calculate_scalable_stat(uint8_t base, uint8_t level, float multiplier);

    static int random_number(const int min, const int max);

    static uint8_t random_number(const uint8_t min, const uint8_t max);

    static int random_from_weighted_probabilities(const std::vector<float>& probabilities);

    static uint8_t calculate_creature_level(uint8_t base, float multiplier);

private:
    static float random_float(const float min, const float max);

    static int get_random_from_item(const uint8_t item);
};


#endif  // CALCULATOR_H
