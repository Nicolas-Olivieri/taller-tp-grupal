#include "deposit_item_event.h"

#include "common/protocol/serializer.h"


DepositItemEventDTO::DepositItemEventDTO(const uint8_t item_id):
        EventDTO(CommandType::DEPOSIT_ITEM), item_id(item_id) {}


size_t DepositItemEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item_id); }


void DepositItemEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
