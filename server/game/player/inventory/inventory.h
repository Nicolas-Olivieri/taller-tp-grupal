#ifndef INVENTORY_H
#define INVENTORY_H

#include <cstdint>
#include <map>
#include <stdexcept>

#include "server/game/stats/stats.h"

#include "equipment.h"


struct InventoryFull: public std::runtime_error {
    InventoryFull(): std::runtime_error("The player's inventory is full.") {}
};


struct ItemNotOwned: public std::runtime_error {
    ItemNotOwned(): std::runtime_error("The player does not have this item in the inventory.") {}
};


struct ItemEquipped: public std::runtime_error {
    ItemEquipped(): std::runtime_error("The player has this item equipped.") {}
};


class Inventory {

    Stats& stats;
    // Mapa de item_id a cantidad en posesión de ese item
    std::map<uint8_t, uint8_t> item_to_amount_map;
    Equipment equipment;
    // TODO agregar maximo de items

public:
    // TODO dejar de hardcodear, creo que podría usarse un constructor default, aprovechandose que el NO_ITEM
    // es 0
    explicit Inventory(Stats& stats);

    void use_item(uint8_t item);

    void acquire_item(uint8_t item);

    void drop_item(uint8_t item);

    Equipment get_equipment() const;

    // consigue el rango de ataque
    int get_range() const;

    int get_attack_cost() const;

private:
    void acquire_new_item(uint8_t item);

    // TODO actualizar los métodos -> ahora son solo equipar y desequipar el arma
    void equip_item(uint8_t item);

    void unequip_item(uint8_t item);

    void use_usable_item(uint8_t item);

    bool is_equipped(uint8_t item);

    void consume_item(uint8_t item);
};


#endif  // INVENTORY_H
