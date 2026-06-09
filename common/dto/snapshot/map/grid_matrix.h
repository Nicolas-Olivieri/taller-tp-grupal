#ifndef GRID_MATRIX_H
#define GRID_MATRIX_H
#include <vector>

#include "common/dto/message.h"
#include "common/dto/snapshot/map/asset_info.h"

struct GridMatrixDTO {
    std::vector<std::vector<bool>> walkable_tiles;
    std::vector<AssetInfoDTO> tiles;

    GridMatrixDTO(const std::vector<std::vector<bool>>& walkable_tiles,
                  const std::vector<AssetInfoDTO>& tiles);
};


#endif  // GRID_MATRIX_H
