#include "buy_event.h"

#include "common/protocol/serializer.h"


BuyEventDTO::BuyEventDTO(const uint8_t item_id): EventDTO(CommandType::BUY_ITEM), item_id(item_id) {}


size_t BuyEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item_id); }


void BuyEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
