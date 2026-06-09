#include "clan_request_response_event.h"

#include "common/protocol/serializer.h"

RequestResponseEventDTO::RequestResponseEventDTO(const std::string& player_name, const bool is_accepted):
        EventDTO(CommandType::CLAN_REQUEST_RESPONSE), player_name(player_name), is_accepted(is_accepted) {}

size_t RequestResponseEventDTO::message_size() const {
    return EventDTO::message_size() + sizeof(uint16_t) + player_name.size() + sizeof(uint8_t);
}

void RequestResponseEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
