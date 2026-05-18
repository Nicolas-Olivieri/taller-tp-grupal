#ifndef DIRECTION_H
#define DIRECTION_H

#include <cstdint>

enum class Direction : uint8_t {
    DOWN = 0x00,
    UP = 0x01,
    LEFT = 0x02,
    RIGHT = 0x03,
    IDLE = 0x04,
};


#endif  // DIRECTION_H
