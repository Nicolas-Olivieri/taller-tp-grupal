#ifndef LOOTINFODTO_H
#define LOOTINFODTO_H

#include "common/dto/message.h"

struct LootInfoDTO: public ProtocolMessageDTO {
    bool is_item;
    uint16_t x;
    uint16_t y;

    LootInfoDTO(bool is_item, uint16_t x, uint16_t y);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // LOOTINFODTO_H
