#include "withdraw_item_event.h"

#include "common/protocol/serializer.h"


WithdrawItemEventDTO::WithdrawItemEventDTO(const uint8_t item_id):
        EventDTO(CommandType::WITHDRAW_ITEM), item_id(item_id) {}


size_t WithdrawItemEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item_id); }


void WithdrawItemEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
