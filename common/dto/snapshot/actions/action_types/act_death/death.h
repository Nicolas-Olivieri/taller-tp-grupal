#ifndef DEATH_DTO_H
#define DEATH_DTO_H

#include <string>

#include "common/dto/message.h"


struct DeathDTO: public ProtocolMessageDTO {
    std::string player_dead;

    DeathDTO();

    explicit DeathDTO(const std::string& player_name);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // DEATH_DTO_H
