#include "deposit_gold_event.h"

#include "common/protocol/serializer.h"


DepositGoldEventDTO::DepositGoldEventDTO(const uint16_t gold_amount):
        EventDTO(CommandType::DEPOSIT_GOLD), gold_amount(gold_amount) {}


size_t DepositGoldEventDTO::message_size() const { return EventDTO::message_size() + sizeof(gold_amount); }


void DepositGoldEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
