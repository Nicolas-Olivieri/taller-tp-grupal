#ifndef RACE_H
#define RACE_H

#include <cstdint>


struct Race {
    const uint8_t health_factor;
    const uint8_t mana_factor;
    const uint8_t recovery_factor;

    explicit Race(uint8_t race_id);
};


#endif  // RACE_H
