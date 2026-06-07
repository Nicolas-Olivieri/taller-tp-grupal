#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <cstdint>

#include "item.h"

#define NO_ITEM 0

struct Equipment {
    uint8_t helmet;
    uint8_t armor;
    uint8_t shield;
    uint8_t weapon;
};

// Functor que define que hacer cuando se trata de desequpar un tipo de Item
class Unequip {
private:
    Equipment& equipment;

    bool unequip_item(uint8_t& equipped, uint8_t item);

public:
    explicit Unequip(Equipment& equipment);

    bool operator()(const Weapon& weapon);
    bool operator()(const Shield& shield);
    bool operator()(const Helmet& helmet);
    bool operator()(const Armor& armor);
    bool operator()(const Usable& usable);
};

#endif  // EQUIPMENT_H
