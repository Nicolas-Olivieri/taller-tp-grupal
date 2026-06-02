#ifndef ALLY_INFO_H
#define ALLY_INFO_H

#include <cstdint>

#include "common/creature_type.h"
#include "common/dto/message.h"


struct AllyInfoDTO: public ProtocolMessageDTO {
    AllyType type;
    uint16_t x;
    uint16_t y;

    AllyInfoDTO(AllyType type, uint16_t x, uint16_t y);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // ALLY_INFO_H
