#include "cheat_get_item_event.h"

#include "common/protocol/serializer.h"

CheatGetItemEventDTO::CheatGetItemEventDTO(uint8_t item): EventDTO(CommandType::CHEAT_ITEM), item(item) {}

size_t CheatGetItemEventDTO::message_size() const { return EventDTO::message_size() + sizeof(item); }

void CheatGetItemEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
