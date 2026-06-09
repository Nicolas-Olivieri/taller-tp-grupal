#ifndef INTERACT_EVENT_H
#define INTERACT_EVENT_H

#include <cstdint>

#include "common/dto/events/event.h"


struct InteractEventDTO: public EventDTO {
    uint16_t target_x;
    uint16_t target_y;

    InteractEventDTO(uint16_t target_x, uint16_t target_y);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // INTERACT_EVENT_H
