#include "heal.h"

#include "common/protocol/serializer.h"


HealDTO::HealDTO(): player_healed("") {}


HealDTO::HealDTO(const std::string& player_healed): player_healed(player_healed) {}


size_t HealDTO::message_size() const { return sizeof(uint16_t) + player_healed.size(); }


void HealDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
