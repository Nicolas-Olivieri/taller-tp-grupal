#ifndef APPEARANCE_H
#define APPEARANCE_H

#include "common/dto/message.h"

// TODO: completar la estructura con el resto de campos necesarios en el futuro
struct AppearanceDTO: public ProtocolMessageDTO {
    uint8_t body;
    uint8_t head;

    AppearanceDTO(uint8_t body, uint8_t head);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // APPEARANCE_H
