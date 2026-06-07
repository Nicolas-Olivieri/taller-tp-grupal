#ifndef CLIENT_MAP_DATA_H
#define CLIENT_MAP_DATA_H
#include <vector>

#include "common/dto/message.h"

#include "asset_info.h"


struct ClientMapDataDTO: public ProtocolMessageDTO {
    uint16_t world_width;
    uint16_t world_height;
    std::vector<AssetInfoDTO> tiles;
    std::vector<AssetInfoDTO> colliders;
    std::vector<AssetInfoDTO> npcs;

    ClientMapDataDTO(uint16_t world_w, uint16_t world_h, const std::vector<AssetInfoDTO>& tiles,
                     const std::vector<AssetInfoDTO>& colliders, const std::vector<AssetInfoDTO>& npcs);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLIENT_MAP_DATA_H
