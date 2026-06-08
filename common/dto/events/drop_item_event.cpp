#include "drop_item_event.h"

#include "common/protocol/serializer.h"


DropItemEventDTO::DropItemEventDTO(const uint8_t item_id):
        EventDTO(CommandType::DROP_ITEM), item_id(item_id) {}


size_t DropItemEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item_id); }


void DropItemEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
