#ifndef GRID_MATRIX_H
#define GRID_MATRIX_H
#include <vector>

#include "tile_info.h"

struct GridMatrixDTO {
    std::vector<std::vector<TileInfoDTO>> tiles_info;

    explicit GridMatrixDTO(const std::vector<std::vector<TileInfoDTO>>& walkable_tiles);
};


#endif  // GRID_MATRIX_H
