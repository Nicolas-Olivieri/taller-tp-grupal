#include "despawn.h"

#include "common/protocol/serializer.h"

DespawnDTO::DespawnDTO(const std::string& player_name): player_despawned(player_name) {}

size_t DespawnDTO::message_size() const { return sizeof(uint16_t) + player_despawned.size(); }

void DespawnDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
