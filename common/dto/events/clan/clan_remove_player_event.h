#ifndef CLAN_REMOVE_PLAYER_EVENT_H
#define CLAN_REMOVE_PLAYER_EVENT_H


#include <string>

#include "common/dto/events/event.h"

struct ClanRemovePlayerEventDTO: public EventDTO {
    std::string player_name;
    bool is_permanent_removal;

    ClanRemovePlayerEventDTO(const std::string& player_name, const bool is_permanent_removal);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_REMOVE_PLAYER_EVENT_H
