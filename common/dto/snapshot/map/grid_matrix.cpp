#include "grid_matrix.h"

#include <vector>

#include "common/protocol/serializer.h"

GridMatrixDTO::GridMatrixDTO(const std::vector<std::vector<TileInfoDTO>>& walkable_tiles):
        tiles_info(walkable_tiles) {}

size_t GridMatrixDTO::message_size() const {
    if (tiles_info.empty()) {
        return 0;
    }
    return tiles_info[0][0].message_size() * tiles_info.size() * tiles_info[0].size();
}

void GridMatrixDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
