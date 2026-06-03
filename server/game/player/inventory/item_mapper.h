#ifndef ITEM_MAPPER_H
#define ITEM_MAPPER_H

#include <cstdint>
#include <vector>

// Clase con metodos estáticos que llaman al config internamente
class ItemMapper {
public:
    static bool is_equipable(uint8_t item_id);

    static bool is_weapon(uint8_t item_id);
    static bool is_armor(uint8_t item_id);
    static bool is_shield(uint8_t item_id);
    static bool is_helmet(uint8_t item_id);

    static bool is_usable(uint8_t item_id);

    static int get_max(uint8_t item_id);

    static int get_min(uint8_t item_id);

    static int get_range(uint8_t item_id);

    static int get_mana_cost(uint8_t item_id);

    static bool is_magic(uint8_t item_id);

    static uint8_t get_usable_type_effect(uint8_t item_id);

    static uint16_t get_usable_effect_amount(uint8_t item_id);

    static uint16_t get_price(uint8_t item_id);

    static std::vector<uint8_t> get_random_priest_items();
};


#endif  // ITEM_MAPPER_H
