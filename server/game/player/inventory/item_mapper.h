#ifndef ITEM_MAPPER_H
#define ITEM_MAPPER_H

#include <cstdint>

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
};


#endif  // ITEM_MAPPER_H
