#ifndef CREATUREINFODTO_H
#define CREATUREINFODTO_H

#include "common/direction.h"
#include "common/dto/message.h"

struct CreatureInfoDTO: public ProtocolMessageDTO {
    uint8_t creature;
    uint8_t variation;
    uint16_t sub_id;
    Direction direction;
    uint16_t x;
    uint16_t y;

    CreatureInfoDTO(uint8_t creature_id, uint8_t variation_id, uint16_t sub_id, const Direction& direction,
                    uint16_t x, uint16_t y);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // CREATUREINFODTO_H
