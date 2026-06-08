#include "use_item_event.h"

#include "common/protocol/serializer.h"


UseItemEventDTO::UseItemEventDTO(const uint8_t item_id): EventDTO(CommandType::USE_ITEM), item_id(item_id) {}


size_t UseItemEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item_id); }


void UseItemEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
