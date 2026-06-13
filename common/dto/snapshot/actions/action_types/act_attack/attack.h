#ifndef ATTACK_DTO_H
#define ATTACK_DTO_H

#include <string>

#include "common/dto/message.h"


struct AttackDTO: public ProtocolMessageDTO {
    std::string attacker;
    uint8_t weapon;
    uint16_t x;
    uint16_t y;
    uint8_t missed;

    AttackDTO();

    explicit AttackDTO(const std::string& attacker, uint8_t weapon, uint16_t x, uint16_t y, uint8_t missed);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // ATTACK_DTO_H
