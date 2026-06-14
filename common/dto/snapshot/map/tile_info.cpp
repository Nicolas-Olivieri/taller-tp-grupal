#include "tile_info.h"

#include "common/protocol/serializer.h"

TileInfoDTO::TileInfoDTO(const bool walkable, const uint8_t biome) : walkable(walkable), biome(biome) {}

size_t TileInfoDTO::message_size() const {
    return sizeof(biome) + sizeof(walkable);
}

void TileInfoDTO::accept(Serializer &serializer) const {
    serializer.serialize(*this);
}
