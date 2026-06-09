#include "clan_remove_player_event.h"

#include "common/protocol/serializer.h"

ClanRemovePlayerEventDTO::ClanRemovePlayerEventDTO(const std::string& player_name,
                                                   const bool is_permanent_removal):
        EventDTO(CommandType::CLAN_REMOVE_PLAYER),
        player_name(player_name),
        is_permanent_removal(is_permanent_removal) {}

size_t ClanRemovePlayerEventDTO::message_size() const {
    return EventDTO::message_size() + sizeof(uint16_t) + player_name.size() + sizeof(uint8_t);
}

void ClanRemovePlayerEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
