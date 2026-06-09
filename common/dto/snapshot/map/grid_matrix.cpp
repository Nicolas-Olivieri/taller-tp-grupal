#include "grid_matrix.h"

#include <utility>
#include <vector>

GridMatrixDTO::GridMatrixDTO(const std::vector<std::vector<bool>>& walkable_tiles,
                             std::vector<std::vector<uint8_t>> tiles_ids):
        walkable_tiles(walkable_tiles), tiles_ids(std::move(tiles_ids)) {}
