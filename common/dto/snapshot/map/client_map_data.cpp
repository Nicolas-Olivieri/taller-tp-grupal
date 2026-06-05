#include "client_map_data.h"

#include "common/protocol/serializer.h"

ClientMapDataDTO::ClientMapDataDTO(const std::vector<AssetInfoDTO> &tiles, const std::vector<AssetInfoDTO> &colliders, const std::vector<AssetInfoDTO> &npcs) :
    tiles(tiles), colliders(colliders), npcs(npcs) {}

size_t ClientMapDataDTO::message_size() const {
    constexpr size_t asset_sz = sizeof(uint8_t)+sizeof(uint16_t)*2;
    return asset_sz*tiles.size() + asset_sz*colliders.size() + asset_sz*npcs.size();
}

void ClientMapDataDTO::accept(Serializer &serializer) const {
    serializer.serialize(*this);
}
