#ifndef PLAYER_STATS_H
#define PLAYER_STATS_H


#include "common/dto/message.h"

struct PlayerStatsDTO: public ProtocolMessageDTO {
    uint16_t max_health;
    uint16_t current_health;
    uint16_t max_mana;
    uint16_t current_mana;
    uint8_t xp_level;
    uint32_t current_xp_amount;

    PlayerStatsDTO(uint16_t max_health, uint16_t current_health, uint16_t max_mana, uint16_t current_mana,
                   uint8_t xp_level, uint32_t current_xp_amount);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // PLAYER_STATS_H
