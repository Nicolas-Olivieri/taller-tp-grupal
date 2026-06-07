#ifndef ITEM_MAPPER_H
#define ITEM_MAPPER_H

#include <cstdint>
#include <vector>

#include "item.h"

// Clase con metodos estáticos que llaman al config internamente
class ItemMapper {
public:
    static Item parse_item(uint8_t item);

    static UsableTypeEffect get_usable_type_effect(uint8_t item_id);

    static uint16_t get_usable_effect_amount(uint8_t item_id);

private:
    static bool is_weapon(uint8_t item_id);

    static bool is_armor(uint8_t item_id);

    static bool is_shield(uint8_t item_id);

    static bool is_helmet(uint8_t item_id);

    static bool is_usable(uint8_t item_id);
};


#endif  // ITEM_MAPPER_H
