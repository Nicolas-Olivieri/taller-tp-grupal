#ifndef INVENTORY_H
#define INVENTORY_H

#include <cstdint>
#include <map>

#include "server/game/equipment.h"
#include "server/game/stats/stats.h"

class Inventory {

    // Mapa de item_id a cantidad en posesión de ese item
    std::map<uint8_t, uint8_t> item_to_amount_map;
    // TODO agregar maximo de items

public:
    // TODO dejar de hardcodear, creo que podría usarse un constructor default, aprovechandose que el NO_ITEM
    // es 0
    explicit Inventory(const Equipment& equipment);

    void use_item(Stats& stats, Equipment& equipment, uint8_t item);

    void acquire_item(uint8_t item);

    void drop_item(Equipment& equipment, uint8_t item);

    // consigue el rango de ataque
    int get_range(const Equipment& equipment) const;

    int get_attack_cost(const Equipment& equipment) const;

private:
    void aquire_new_item(uint8_t item);

    // TODO actualizar los métodos -> ahora son solo equipar y desequipar el arma
    void equip_item(Equipment& equipment, uint8_t item);

    void unequip_item(Equipment& equipment, uint8_t item);

    void use_usable_item(Stats& stats, Equipment& equpiment, uint8_t item);

    bool is_equipped(const Equipment& equipment, uint8_t item);

    void consume_item(Equipment& equipment, uint8_t item);
};


#endif  // INVENTORY_H
