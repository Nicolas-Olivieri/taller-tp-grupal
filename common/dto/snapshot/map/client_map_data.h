#ifndef CLIENT_MAP_DATA_H
#define CLIENT_MAP_DATA_H
#include "asset_info.h"
#include "common/dto/message.h"


struct ClientMapDataDTO : public ProtocolMessageDTO{
    std::vector<AssetInfoDTO> tiles;
    std::vector<AssetInfoDTO> colliders;
    std::vector<AssetInfoDTO> npcs;

    ClientMapDataDTO(const std::vector<AssetInfoDTO> &tiles, const std::vector<AssetInfoDTO> &colliders, const std::vector<AssetInfoDTO> &npcs);

    size_t message_size() const override;

    void accept(Serializer &serializer) const override;
};


#endif //CLIENT_MAP_DATA_H
