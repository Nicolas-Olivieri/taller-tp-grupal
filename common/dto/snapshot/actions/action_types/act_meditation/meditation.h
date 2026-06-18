#ifndef MEDITATION_DTO_H
#define MEDITATION_DTO_H

#include <string>

#include "common/dto/message.h"


struct MeditationDTO: public ProtocolMessageDTO {
    std::string player_meditating;

    MeditationDTO();

    explicit MeditationDTO(const std::string& player_meditating);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // MEDITATION_DTO_H
