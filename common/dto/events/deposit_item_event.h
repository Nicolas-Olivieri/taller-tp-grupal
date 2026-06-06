#ifndef DEPOSIT_ITEM_EVENT_DTO_H
#define DEPOSIT_ITEM_EVENT_DTO_H

#include "event.h"


struct DepositItemEventDTO: public EventDTO {
    uint8_t item_id;

    explicit DepositItemEventDTO(uint8_t item_id);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // DEPOSIT_ITEM_EVENT_DTO_H
