#include "grid_matrix.h"

#include <vector>

GridMatrixDTO::GridMatrixDTO(const std::vector<std::vector<TileInfoDTO>>& walkable_tiles):
        tiles_info(walkable_tiles) {}
