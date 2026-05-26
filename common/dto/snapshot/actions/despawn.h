#ifndef DESPAWNDTO_H
#define DESPAWNDTO_H

#include <string>

#include "common/dto/message.h"

struct DespawnDTO: public ProtocolMessageDTO {
    std::string player_despawned;

    explicit DespawnDTO(const std::string& player_name);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // DESPAWNDTO_H
