#ifndef DEPOSIT_GOLD_EVENT_DTO_H
#define DEPOSIT_GOLD_EVENT_DTO_H

#include "common/dto/events/event.h"


struct DepositGoldEventDTO: public EventDTO {
    uint16_t gold_amount;

    explicit DepositGoldEventDTO(uint16_t gold_amount);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // DEPOSIT_GOLD_EVENT_DTO_H
