#include "creatureinfo.h"

#include "common/protocol/serializer.h"

CreatureInfoDTO::CreatureInfoDTO(uint8_t creature_id, uint8_t variation_id, uint16_t sub_id,
                                 const Direction& direction, uint16_t x, uint16_t y):
        creature(creature_id), variation(variation_id), sub_id(sub_id), direction(direction), x(x), y(y) {}

size_t CreatureInfoDTO::message_size() const {
    return sizeof(uint8_t) * 2 + sizeof(uint16_t) * 3 + sizeof(Direction);
}

void CreatureInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
