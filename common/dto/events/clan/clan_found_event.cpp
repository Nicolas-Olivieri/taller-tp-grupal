#include "clan_found_event.h"

#include "common/protocol/serializer.h"

ClanFoundEventDTO::ClanFoundEventDTO(const std::string& clan_name):
        EventDTO(CommandType ::CLAN_FOUND), clan_name(clan_name) {}

size_t ClanFoundEventDTO::message_size() const {
    return EventDTO::message_size() + sizeof(uint16_t) + clan_name.size();
}

void ClanFoundEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
