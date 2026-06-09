#ifndef WITHDRAW_GOLD_EVENT_H
#define WITHDRAW_GOLD_EVENT_H

#include "common/dto/events/event.h"


struct WithdrawGoldEventDTO: public EventDTO {
    uint16_t gold_amount;

    explicit WithdrawGoldEventDTO(uint16_t gold_amount);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // WITHDRAW_GOLD_EVENT_H
