#include "sell_event.h"

#include "common/protocol/serializer.h"


SellEventDTO::SellEventDTO(const uint8_t item_id): EventDTO(CommandType::SELL_ITEM), item_id(item_id) {}


size_t SellEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item_id); }


void SellEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
