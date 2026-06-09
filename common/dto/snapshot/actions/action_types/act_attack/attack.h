#ifndef ATTACK_DTO_H
#define ATTACK_DTO_H

#include <string>

#include "common/dto/message.h"


struct AttackDTO: public ProtocolMessageDTO {
    std::string attacker;
    uint8_t weapon;

    AttackDTO();

    explicit AttackDTO(const std::string& attacker, uint8_t weapon);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // ATTACK_DTO_H
