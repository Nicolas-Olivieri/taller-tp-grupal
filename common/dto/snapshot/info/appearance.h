#ifndef APPEARANCEDTO_H
#define APPEARANCEDTO_H

#include <string>

#include "common/dto/message.h"

struct AppearanceDTO: public ProtocolMessageDTO {
    uint8_t body;
    uint8_t head;

    AppearanceDTO();

    AppearanceDTO(uint8_t body, uint8_t head);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // APPEARANCEDTO_H
