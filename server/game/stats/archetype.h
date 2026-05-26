#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include <cstdint>


struct Archetype {
    const uint8_t health_factor;
    const uint8_t mana_factor;
    const uint8_t meditation_factor;

    explicit Archetype(uint8_t health_factor, uint8_t mana_factor, uint8_t meditation_factor);
};


#endif  // ARCHETYPE_H
