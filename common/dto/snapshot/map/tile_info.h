#ifndef TILE_INFO_H
#define TILE_INFO_H

#include <cstdint>

struct TileInfoDTO {
    bool walkable;
    uint8_t biome;

    TileInfoDTO(bool walkable, uint8_t biome);
};


#endif  // TILE_INFO_H
