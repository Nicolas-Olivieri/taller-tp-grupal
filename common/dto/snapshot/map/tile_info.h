#ifndef TILE_INFO_H
#define TILE_INFO_H

#include <cstdint>

struct TileInfoDTO {
    bool walkable;
    uint8_t floor;

    TileInfoDTO(bool walkable, uint8_t floor);
};


#endif  // TILE_INFO_H
