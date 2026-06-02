#include "inventory.h"

#include <cassert>

#include "item_mapper.h"

// ver comentario en el .h
Inventory::Inventory(): equipment({NO_ITEM, NO_ITEM, NO_ITEM, 1}) { item_to_amount_map[1] = 1; }

void Inventory::use_item(uint8_t item) {
    if (!item_to_amount_map.contains(item)) {
        return;
    }

    if (ItemMapper::is_equipable(item)) {
        equip_item(item);
    }
    if (ItemMapper::is_usable(item)) {
        use_usable_item(item);
    }
}

void Inventory::drop_item(uint8_t item) {
    if (!item_to_amount_map.contains(item)) {
        // TODO se quiso soltar un item q no tengo -> Excepción?
        return;
    }

    item_to_amount_map[item]--;

    if (item_to_amount_map[item] == 0) {
        if (is_equipped(item)) {
            unequip_item(item);
        }
        item_to_amount_map.extract(item);
    }
}

void Inventory::acquire_item(uint8_t item) {
    if (!item_to_amount_map.contains(item)) {
        aquire_new_item(item);
        return;
    }
    item_to_amount_map[item]++;
}

void Inventory::aquire_new_item(uint8_t item) { item_to_amount_map[item] = 1; }

void Inventory::equip_item(uint8_t item) { equipment.weapon = item; }

void Inventory::unequip_item(uint8_t /* item */) {
    // TODO debería verificar que lo tengo equipado antes
    equipment.weapon = NO_ITEM;
}

bool Inventory::is_equipped(uint8_t item) {
    return equipment.armor == item || equipment.shield == item || equipment.helmet == item ||
           equipment.weapon == item;
}

Equipment Inventory::get_equipment() const { return equipment; }

void Inventory::use_usable_item(uint8_t item) {
    assert(ItemMapper::is_usable(item));
    // TODO continuar
}

int Inventory::get_range() const {
    if (equipment.weapon == NO_ITEM)
        return 1;

    return ItemMapper::get_range(equipment.weapon);
}

int Inventory::get_attack_cost() const { return ItemMapper::get_mana_cost(equipment.weapon); }
