#ifndef CHEAT_GET_ITEM_EVENT_H
#define CHEAT_GET_ITEM_EVENT_H

#include "common/dto/events/event.h"

struct CheatGetItemEventDTO: public EventDTO {
    uint8_t item;

    explicit CheatGetItemEventDTO(uint8_t item);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // CHEAT_GET_ITEM_EVENT_H
