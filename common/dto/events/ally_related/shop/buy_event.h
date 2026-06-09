#ifndef BUY_EVENT_DTO_H
#define BUY_EVENT_DTO_H

#include <cstdint>

#include "common/dto/events/event.h"


struct BuyEventDTO: public EventDTO {
    uint8_t item_id;

    explicit BuyEventDTO(uint8_t item_id);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // BUY_EVENT_DTO_H
