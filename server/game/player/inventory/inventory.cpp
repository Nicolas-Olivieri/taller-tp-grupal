#include "inventory.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "item_mapper.h"

// ver comentario en el .h
Inventory::Inventory(const Equipment& equipment) {
    item_to_amount_map[equipment.weapon] = 1;  // TODO: sacar este hardcodeo
}

void Inventory::use_item(Stats& stats, Equipment& equipment, uint8_t item) {
    if (!item_to_amount_map.contains(item)) {
        return;
    }

    if (ItemMapper::is_equipable(item)) {
        equip_item(equipment, item);
    } else if (ItemMapper::is_usable(item)) {
        use_usable_item(stats, equipment, item);
    }
}

void Inventory::drop_item(Equipment& equipment, uint8_t item) {
    if (!item_to_amount_map.contains(item)) {
        throw ItemNotOwned();
    }

    if (is_equipped(item)) {
        throw ItemEquipped();
    }

    consume_item(equipment, item);
    // TODO lo debería soltar/devolver??
}

void Inventory::acquire_item(uint8_t item) {
    if (!item_to_amount_map.contains(item)) {
        aquire_new_item(item);
        return;
    }
    item_to_amount_map[item]++;
}

void Inventory::aquire_new_item(uint8_t item) { item_to_amount_map[item] = 1; }

void Inventory::equip_item(Equipment& equipment, uint8_t item) {
    // TODO: considerar todas las ranuras
    equipment.weapon = item;
}

void Inventory::unequip_item(Equipment& equipment, uint8_t /* item */) {
    // TODO debería verificar que lo tengo equipado antes
    // TODO: considerar todas las ranuras
    equipment.weapon = NO_ITEM;
}

bool Inventory::is_equipped(const Equipment& equipment, uint8_t item) {
    return equipment.armor == item || equipment.shield == item || equipment.helmet == item ||
           equipment.weapon == item;
}

void Inventory::use_usable_item(Stats& stats, Equipment& equpiment, uint8_t item) {
    assert(ItemMapper::is_usable(item));
    assert(item_to_amount_map.contains(item));

    const uint16_t effect_amount = ItemMapper::get_usable_effect_amount(item);

    switch (static_cast<UsableTypeEffect>(ItemMapper::get_usable_type_effect(item))) {
        case UsableTypeEffect::HEALTH:
            stats.health.recover(effect_amount);
            break;
        case UsableTypeEffect::MANA:
            stats.mana.recover(effect_amount);
            break;
        default:
            throw std::runtime_error("Inventory encontró un tipo inválido de efecto de usable");
    }

    consume_item(equpiment, item);
}

int Inventory::get_range(const Equipment& equipment) const {
    if (equipment.weapon == NO_ITEM)
        return 1;

    return ItemMapper::get_range(equipment.weapon);
}

int Inventory::get_attack_cost(const Equipment& equipment) const {
    return ItemMapper::get_mana_cost(equipment.weapon);
}

void Inventory::consume_item(Equipment& equipment, uint8_t item) {
    assert(item_to_amount_map.contains(item));

    item_to_amount_map[item]--;

    if (item_to_amount_map[item] == 0) {
        if (is_equipped(equipment, item)) {
            unequip_item(equipment, item);
        }
        item_to_amount_map.extract(item);
    }
}
