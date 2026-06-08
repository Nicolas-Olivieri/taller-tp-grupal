#ifndef CLAN_FOUND_EVENT_H
#define CLAN_FOUND_EVENT_H


#include <string>

#include "event.h"

struct ClanFoundEventDTO: public EventDTO {
    std::string clan_name;

    explicit ClanFoundEventDTO(const std::string& clan_name);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_FOUND_EVENT_H
