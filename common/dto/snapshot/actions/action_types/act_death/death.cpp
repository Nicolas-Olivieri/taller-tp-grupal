#include "death.h"

#include "common/protocol/serializer.h"


DeathDTO::DeathDTO(const std::string& player_name): player_dead(player_name) {}


size_t DeathDTO::message_size() const { return sizeof(uint16_t) + player_dead.size(); }


void DeathDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
