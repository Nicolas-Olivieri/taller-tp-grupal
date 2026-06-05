#include "client_map_data.h"

#include "common/protocol/serializer.h"

ClientMapDataDTO::ClientMapDataDTO(const uint16_t world_w, const uint16_t world_h, const std::vector<AssetInfoDTO> &tiles,
                                   const std::vector<AssetInfoDTO> &colliders, const std::vector<AssetInfoDTO> &npcs) :
    world_width(world_w), world_height(world_h), tiles(tiles), colliders(colliders), npcs(npcs) {}

size_t ClientMapDataDTO::message_size() const {
    constexpr size_t asset_sz = sizeof(uint8_t)+sizeof(uint16_t)*2;
    return sizeof(world_width) + sizeof(world_height) + sizeof(uint16_t)*3 + asset_sz*tiles.size() + asset_sz*colliders.size() + asset_sz*npcs.size();
}

void ClientMapDataDTO::accept(Serializer &serializer) const {
    serializer.serialize(*this);
}
