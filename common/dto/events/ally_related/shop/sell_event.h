#ifndef SELL_EVENT_DTO_H
#define SELL_EVENT_DTO_H

#include <cstdint>

#include "common/dto/events/event.h"


struct SellEventDTO: public EventDTO {
    uint8_t item_id;

    explicit SellEventDTO(uint8_t item_id);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // SELL_EVENT_DTO_H
