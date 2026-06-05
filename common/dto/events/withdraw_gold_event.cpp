#include "withdraw_gold_event.h"

#include "common/protocol/serializer.h"


WithdrawGoldEventDTO::WithdrawGoldEventDTO(const uint16_t gold_amount):
        EventDTO(CommandType::DEPOSIT_GOLD), gold_amount(gold_amount) {}


size_t WithdrawGoldEventDTO::message_size() const { return EventDTO::message_size() + sizeof(gold_amount); }


void WithdrawGoldEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
