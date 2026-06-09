#include "unequip_item_event.h"

#include "common/protocol/serializer.h"


UnequipItemEventDTO::UnequipItemEventDTO(const uint8_t item_id):
        EventDTO(CommandType::UNEQUIP_ITEM), item_id(item_id) {}


size_t UnequipItemEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item_id); }


void UnequipItemEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
