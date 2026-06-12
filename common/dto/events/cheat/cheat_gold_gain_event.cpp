#include "cheat_gold_gain_event.h"

#include "common/protocol/serializer.h"

CheatGoldGainEventDTO::CheatGoldGainEventDTO(uint16_t amount): EventDTO(CommandType::CHEAT_GOLD), amount(amount) {}

size_t CheatGoldGainEventDTO::message_size() const {
    return EventDTO::message_size() + sizeof(amount);
}

void CheatGoldGainEventDTO::accept(Serializer& serializer) const {
    serializer.serialize(*this);
}
