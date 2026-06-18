#ifndef DESPAWN_DTO_H
#define DESPAWN_DTO_H

#include <string>

#include "common/dto/message.h"

struct DespawnDTO: public ProtocolMessageDTO {
    std::string player_despawned;

    DespawnDTO();

    explicit DespawnDTO(const std::string& player_name);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // DESPAWN_DTO_H
