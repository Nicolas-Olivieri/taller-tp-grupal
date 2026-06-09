#include "inventory.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

#include "server/game/items/item_mapper.h"
#include "server/persistance/playerdata.h"  // TODO: borrar después de meter el TOML

Inventory::Inventory(std::span<const uint8_t> items_id, std::span<const uint8_t> items_amount):
        max_item_amount(INVENTORY_AMOUNT) {
    assert(items_id.size() == items_amount.size());
    assert(items_id.size() <= max_item_amount);

    for (size_t i = 0; i < items_id.size(); i++) {
        if (items_amount[i] > 0)
            items_amounts[items_id[i]] = items_amount[i];
    }
}

Inventory::Inventory(): max_item_amount(INVENTORY_AMOUNT) {}

void Inventory::use_item(Stats& stats, Equipment& equipment, uint8_t item_id) {
    if (!items_amounts.contains(item_id))
        throw ItemNotOwned();

    Item item = ItemMapper::parse_item(item_id);
    uint8_t unequipped = std::visit(Use{stats, equipment}, item);

    if (unequipped != NO_ITEM)
        add_amount_safely(unequipped);

    consume_item(item_id);
}

void Inventory::drop_item(const Equipment& equipment, uint8_t item) {
    if (!items_amounts.contains(item))
        throw ItemNotOwned();

    if (!items_amounts.contains(item) && is_equipped(equipment, item))
        throw ItemEquipped();

    consume_item(item);
}

void Inventory::acquire_item(uint8_t item) {
    if (!items_amounts.contains(item) && items_amounts.size() >= max_item_amount)
        throw InventoryFull();

    add_amount_safely(item);
}

void Inventory::unequip_item(Equipment& equipment, uint8_t item_id) {
    if (not items_amounts.contains(item_id) and items_amounts.size() >= max_item_amount) {
        throw InventoryFull();
    }

    Item item = ItemMapper::parse_item(item_id);
    bool could_unequip = std::visit(Unequip{equipment}, item);

    if (could_unequip)
        add_amount_safely(item_id);
}

bool Inventory::is_equipped(const Equipment& equipment, uint8_t item) const {
    return equipment.armor == item || equipment.shield == item || equipment.helmet == item ||
           equipment.weapon == item;
}

void Inventory::consume_item(uint8_t item) {
    assert(items_amounts.contains(item));

    items_amounts[item]--;

    if (items_amounts[item] == 0) {
        items_amounts.extract(item);
    }
}

void Inventory::add_amount_safely(uint8_t item) {
    if (items_amounts[item] == UINT8_MAX)
        throw SlotFull();

    items_amounts[item]++;
}

const std::unordered_map<uint8_t, uint8_t>& Inventory::get_items() const { return this->items_amounts; }

void Inventory::clear() { items_amounts.clear(); }
