#ifndef UNEQUIP_ITEM_EVENT_DTO_H
#define UNEQUIP_ITEM_EVENT_DTO_H

#include "event.h"


struct UnequipItemEventDTO: public EventDTO {
    uint8_t item_id;

    explicit UnequipItemEventDTO(uint8_t item_id);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // UNEQUIP_ITEM_EVENT_DTO_H
