#ifndef INVENTORY_H
#define INVENTORY_H

#include <cstdint>
#include <map>

#include "equipment.h"


class Inventory {

    // Mapa de item_id a cantidad en posesión de ese item
    std::map<uint8_t, uint8_t> item_to_amount_map;
    Equipment equipment;
    // TODO agregar maximo de items

public:
    // TODO dejar de hardcodear, creo que podría usarse un constructor default, aprovechandose que el NO_ITEM
    // es 0
    Inventory();

    void use_item(uint8_t item);

    void acquire_item(uint8_t item);

    void drop_item(uint8_t item);

    Equipment get_equipment() const;

    // consigue el rango de ataque
    int get_range() const;

    int get_attack_cost() const;

private:
    void aquire_new_item(uint8_t item);

    // TODO actualizar los métodos -> ahora son solo equipar y desequipar el arma
    void equip_item(uint8_t item);

    void unequip_item(uint8_t item);

    void use_usable_item(uint8_t item);

    bool is_equipped(uint8_t item);
};


#endif  // INVENTORY_H
