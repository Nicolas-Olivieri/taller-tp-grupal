#include "resurrection.h"

#include "common/protocol/serializer.h"


ResurrectionDTO::ResurrectionDTO(const std::string& player_name, const AppearanceDTO& appearance):
        player_resurrected(player_name), original_appearance(appearance) {}

size_t ResurrectionDTO::message_size() const {
    return sizeof(uint16_t) + player_resurrected.size() + original_appearance.message_size();
}

void ResurrectionDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
