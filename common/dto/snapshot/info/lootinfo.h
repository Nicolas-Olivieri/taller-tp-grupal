#ifndef LOOTINFODTO_H
#define LOOTINFODTO_H

#include "common/dto/message.h"
#include "common/loot_type.h"

struct LootInfoDTO: public ProtocolMessageDTO {
    LootType type;
    uint16_t x;
    uint16_t y;

    LootInfoDTO(LootType type, uint16_t x, uint16_t y);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // LOOTINFODTO_H
