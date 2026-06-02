#ifndef CHATEVENTDTO_H
#define CHATEVENTDTO_H

#include <string>

#include "event.h"

struct ChatEventDTO: public EventDTO {
    std::string receiver;
    std::string content;

    ChatEventDTO(const std::string& receiver, const std::string& content);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // CHATEVENTDTO_H
