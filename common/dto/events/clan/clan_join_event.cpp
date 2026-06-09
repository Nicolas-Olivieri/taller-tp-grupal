
#include "clan_join_event.h"

#include "common/protocol/serializer.h"

ClanJoinEventDTO::ClanJoinEventDTO(const std::string& clan_name):
        EventDTO(CommandType::CLAN_JOIN), clan_name(clan_name) {}

size_t ClanJoinEventDTO::message_size() const {
    return EventDTO::message_size() + sizeof(uint16_t) + clan_name.size();
}

void ClanJoinEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
