#ifndef CHEAT_EXPERIENCE_SET_EVENT_H
#define CHEAT_EXPERIENCE_SET_EVENT_H

#include <cstdint>

#include "common/dto/events/event.h"

struct CheatExperienceSetEventDTO: public EventDTO {
    uint8_t level;

    explicit CheatExperienceSetEventDTO(uint8_t level);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CHEAT_EXPERIENCE_SET_EVENT_H
