#ifndef APPEARANCEDTO_H
#define APPEARANCEDTO_H

#include <string>

#include "common/dto/message.h"

// TODO: completar la estructura con el resto de campos necesarios en el futuro
struct AppearanceDTO: public ProtocolMessageDTO {
    uint8_t body;
    uint8_t head;

    // TODO Constructor default para conseguir una apariencia default hasta que podamos conseguirla en base al
    // jugador
    AppearanceDTO();

    AppearanceDTO(uint8_t body, uint8_t head);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // APPEARANCEDTO_H
