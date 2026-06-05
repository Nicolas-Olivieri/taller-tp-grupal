#ifndef ASSET_INFO_H
#define ASSET_INFO_H
#include "common/dto/message.h"


struct AssetInfoDTO : public ProtocolMessageDTO {
    uint8_t id;
    uint16_t x;
    uint16_t y;

    AssetInfoDTO(uint8_t id, uint16_t x, uint16_t y);

    size_t message_size() const override;

    void accept(Serializer &serializer) const override;
};


#endif //ASSET_INFO_H
