#include "attack.h"

#include "common/protocol/serializer.h"


AttackDTO::AttackDTO(): weapon(0), x(0), y(0), missed(0) {}


AttackDTO::AttackDTO(const uint8_t weapon, const uint16_t x, const uint16_t y, const uint8_t missed):
        weapon(weapon), x(x), y(y), missed(missed) {}


size_t AttackDTO::message_size() const { return sizeof(uint8_t) + sizeof(uint16_t) * 2 + sizeof(missed); }


void AttackDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
