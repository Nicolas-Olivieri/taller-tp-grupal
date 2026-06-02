#ifndef EQUIPMENT_H
#define EQUIPMENT_H


#include <cstdint>

#define NO_ITEM 0

struct Equipment {
    uint8_t helmet;
    uint8_t armor;
    uint8_t shield;
    uint8_t weapon;
};

#endif  // EQUIPMENT_H
