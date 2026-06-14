#ifndef TILE_INFO_H
#define TILE_INFO_H
#include "common/dto/message.h"


struct TileInfoDTO : public ProtocolMessageDTO {
    bool walkable;
    uint8_t biome;

    TileInfoDTO(bool walkable, uint8_t biome);

    size_t message_size() const override;

    void accept(Serializer &serializer) const override;
};


#endif //TILE_INFO_H
