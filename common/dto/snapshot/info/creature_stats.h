#ifndef CREATURE_STATS_DTO_H
#define CREATURE_STATS_DTO_H

#include "common/dto/message.h"


struct CreatureStatsDTO: public ProtocolMessageDTO {
    uint16_t max_health;
    uint16_t current_health;
    uint8_t xp_level;

    CreatureStatsDTO(uint16_t max_health, uint16_t current_health, uint8_t xp_level);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CREATURE_STATS_DTO_H
