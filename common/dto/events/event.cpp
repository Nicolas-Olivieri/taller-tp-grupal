#include "event.h"

#include "common/protocol/serializer.h"

EventDTO::EventDTO(const CommandType& command): command(command) {}

size_t EventDTO::message_size() const { return sizeof(command); }

void EventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
