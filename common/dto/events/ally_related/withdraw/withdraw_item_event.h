#ifndef WITHDRAW_ITEM_EVENT_DTO_H
#define WITHDRAW_ITEM_EVENT_DTO_H

#include "common/dto/events/event.h"


struct WithdrawItemEventDTO: public EventDTO {
    uint8_t item_id;

    explicit WithdrawItemEventDTO(uint8_t item_id);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // WITHDRAW_ITEM_EVENT_DTO_H
