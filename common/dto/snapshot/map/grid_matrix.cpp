#include "grid_matrix.h"

#include <iostream>
#include <utility>
#include <vector>

GridMatrixDTO::GridMatrixDTO(const std::vector<std::vector<bool>>& walkable_tiles,
                             const std::vector<AssetInfoDTO>& tiles):
        walkable_tiles(walkable_tiles), tiles(tiles) {}
