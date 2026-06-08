#ifndef USE_ITEM_EVENT_DTO_H
#define USE_ITEM_EVENT_DTO_H

#include "event.h"


struct UseItemEventDTO: public EventDTO {
    uint8_t item_id;

    explicit UseItemEventDTO(uint8_t item_id);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // USE_ITEM_EVENT_DTO_H
