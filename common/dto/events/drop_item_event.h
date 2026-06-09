#ifndef DROP_ITEM_EVENT_DTO_H
#define DROP_ITEM_EVENT_DTO_H

#include "event.h"


struct DropItemEventDTO: public EventDTO {
    uint8_t item_id;

    explicit DropItemEventDTO(uint8_t item_id);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // DROP_ITEM_EVENT_DTO_H
