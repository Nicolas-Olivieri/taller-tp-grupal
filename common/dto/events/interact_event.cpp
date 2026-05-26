#include "interact_event.h"

#include "common/protocol/serializer.h"


InteractEventDTO::InteractEventDTO(const uint16_t target_x, const uint16_t target_y):
        EventDTO(CommandType::INTERACT), target_x(target_x), target_y(target_y) {}


size_t InteractEventDTO::message_size() const {
    return EventDTO::message_size() + sizeof(target_x) + sizeof(target_y);
}


void InteractEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
