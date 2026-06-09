#include "moveevent.h"

#include "common/protocol/serializer.h"

MoveEventDTO::MoveEventDTO(Direction direction): EventDTO(CommandType::MOVE), direction(direction) {}

size_t MoveEventDTO::message_size() const { return EventDTO::message_size() + sizeof(direction); }

void MoveEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
