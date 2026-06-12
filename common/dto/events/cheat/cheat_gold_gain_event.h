#ifndef CHEAT_GOLD_GAIN_EVENT_H
#define CHEAT_GOLD_GAIN_EVENT_H

#include "common/dto/events/event.h"

struct CheatGoldGainEventDTO : public EventDTO {
    // Con signo, así si se envía un número negativo podemos reducita la cantidad
    uint16_t amount;
    
    explicit CheatGoldGainEventDTO(uint16_t amount);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif // CHEAT_GOLD_GAIN_EVENT_H
