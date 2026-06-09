#ifndef HEAL_DTO_H
#define HEAL_DTO_H

#include <string>

#include "common/dto/message.h"


struct HealDTO: public ProtocolMessageDTO {
    std::string player_healed;

    HealDTO();

    explicit HealDTO(const std::string& player_healed);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // HEAL_DTO_H
