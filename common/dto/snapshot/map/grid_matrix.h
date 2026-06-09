#ifndef GRID_MATRIX_H
#define GRID_MATRIX_H
#include <vector>

#include "common/dto/message.h"

struct GridMatrixDTO {
    std::vector<std::vector<bool>> walkable_tiles;
    std::vector<std::vector<uint8_t>> tiles_ids;

    GridMatrixDTO(const std::vector<std::vector<bool>>& walkable_tiles,
                  std::vector<std::vector<uint8_t>> tiles_ids);
};


#endif  // GRID_MATRIX_H
