#include "create_player.h"

#include "common/protocol/serializer.h"

CreatePlayerDTO::CreatePlayerDTO(const uint8_t body, const uint8_t head, const uint8_t archetype,
                                 const uint8_t race):
        appearance(AppearanceDTO(body, head)), archetype(archetype), race(race) {}

size_t CreatePlayerDTO::message_size() const {
    return sizeof(appearance.body) + sizeof(appearance.head) + sizeof(archetype) + sizeof(race);
}

void CreatePlayerDTO::accept(Serializer& serializer) const { return serializer.serialize(*this); }
