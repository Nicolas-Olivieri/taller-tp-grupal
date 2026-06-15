#include "tile_info.h"

#include "common/protocol/serializer.h"

TileInfoDTO::TileInfoDTO(const bool walkable, const uint8_t biome): walkable(walkable), biome(biome) {}
