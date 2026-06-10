#include "grid_matrix.h"

#include <vector>

#include "common/protocol/serializer.h"

GridMatrixDTO::GridMatrixDTO(const std::vector<std::vector<bool>>& walkable_tiles):
        walkable_tiles(walkable_tiles) {}

size_t GridMatrixDTO::message_size() const {
    if (walkable_tiles.empty()) {
        return 0;
    }
    return sizeof(uint8_t) * walkable_tiles.size() * walkable_tiles[0].size();
}

void GridMatrixDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
