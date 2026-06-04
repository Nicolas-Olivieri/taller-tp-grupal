#ifndef EXISTENCE_DTO_H
#define EXISTENCE_DTO_H
#include "common/dto/message.h"


struct ExistenceDTO : public ProtocolMessageDTO {
    uint8_t user_exists;

    explicit ExistenceDTO(uint8_t user_exists);

    size_t message_size() const override;

    void accept(Serializer &serializer) const override;
};


#endif //EXISTENCE_DTO_H
