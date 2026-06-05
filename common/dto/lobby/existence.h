#ifndef EXISTENCE_DTO_H
#define EXISTENCE_DTO_H
#include "common/dto/message.h"


struct ExistenceDTO: public ProtocolMessageDTO {
    uint8_t user_exists;
    uint8_t user_connected;

    explicit ExistenceDTO(const uint8_t user_exists, const uint8_t user_connected);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // EXISTENCE_DTO_H
