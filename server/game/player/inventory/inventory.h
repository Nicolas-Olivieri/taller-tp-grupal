#ifndef INVENTORY_H
#define INVENTORY_H

#include <cstdint>
#include <span>
#include <stdexcept>
#include <unordered_map>

#include "server/game/items/equipment.h"
#include "server/game/stats/stats.h"


struct InventoryFull: public std::runtime_error {
    InventoryFull(): std::runtime_error("The player's inventory is full.") {}
};

struct SlotFull: public std::runtime_error {
    SlotFull(): std::runtime_error("The player cannot add more items of this type.") {}
};


struct ItemNotOwned: public std::runtime_error {
    ItemNotOwned(): std::runtime_error("The player does not have this item in the inventory.") {}
};


struct ItemEquipped: public std::runtime_error {
    ItemEquipped(): std::runtime_error("The player has this item equipped.") {}
};

class Inventory {
    // Mapa de item_id a cantidad en posesión de ese item
    std::unordered_map<uint8_t, uint8_t> items_amounts;
    uint8_t max_item_amount;
    // TODO agregar maximo de items

public:
    Inventory();

    Inventory(std::span<const uint8_t> items_id, std::span<const uint8_t> items_amount);

    void use_item(Stats& stats, Equipment& equipment, uint8_t item);

    void unequip_item(Equipment& equipment, uint8_t item);

    void acquire_item(uint8_t item);

    void drop_item(const Equipment& equipment, uint8_t item);

    const std::unordered_map<uint8_t, uint8_t>& get_items() const;

private:
    bool is_equipped(const Equipment& equipment, uint8_t item) const;

    void consume_item(uint8_t item);

    void add_amount_safely(uint8_t item);
};


#endif  // INVENTORY_H
