#ifndef CLAN_JOIN_EVENT_H
#define CLAN_JOIN_EVENT_H


#include <string>

#include "common/dto/events/event.h"

struct ClanJoinEventDTO: public EventDTO {
    std::string clan_name;

    explicit ClanJoinEventDTO(const std::string& clan_name);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_JOIN_EVENT_H
