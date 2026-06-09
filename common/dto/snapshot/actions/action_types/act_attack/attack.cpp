#include "attack.h"

#include "common/protocol/serializer.h"


AttackDTO::AttackDTO(): attacker(""), weapon(0) {}


AttackDTO::AttackDTO(const std::string& attacker, const uint8_t weapon): attacker(attacker), weapon(weapon) {}


size_t AttackDTO::message_size() const { return sizeof(uint16_t) + attacker.size() + sizeof(uint8_t); }


void AttackDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
