#ifndef CLAN_REQUEST_RESPONSE_EVENT_H
#define CLAN_REQUEST_RESPONSE_EVENT_H

#include <string>

#include "common/dto/events/event.h"

struct RequestResponseEventDTO: public EventDTO {
    std::string player_name;
    bool is_accepted;

    RequestResponseEventDTO(const std::string& player_name, const bool is_accepted);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_REQUEST_RESPONSE_EVENT_H
