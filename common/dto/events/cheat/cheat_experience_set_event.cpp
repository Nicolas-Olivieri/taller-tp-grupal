#include "cheat_experience_set_event.h"

#include "common/protocol/serializer.h"

CheatExperienceSetEventDTO::CheatExperienceSetEventDTO(uint8_t level):
        EventDTO(CommandType::CHEAT_XP), level(level) {}

size_t CheatExperienceSetEventDTO::message_size() const { return EventDTO::message_size() + sizeof(level); }

void CheatExperienceSetEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
