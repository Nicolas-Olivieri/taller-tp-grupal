#ifndef APPEARANCEDTO_H
#define APPEARANCEDTO_H

#include <string>

#include "common/dto/message.h"

// TODO: completar la estructura con el resto de campos necesarios en el futuro
struct AppearanceDTO: public ProtocolMessageDTO {
    uint8_t body;
    uint8_t head;
    std::string name;

    AppearanceDTO(uint8_t body, uint8_t head, const std::string& name);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // APPEARANCEDTO_H
