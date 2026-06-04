
#include "player_stats.h"

#include "common/protocol/serializer.h"

size_t PlayerStatsDTO::message_size() const {
    return sizeof(uint16_t) * 2 + sizeof(uint16_t) * 2 + sizeof(uint8_t) + sizeof(uint32_t) * 2;
}

void PlayerStatsDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }

PlayerStatsDTO::PlayerStatsDTO(uint16_t max_health, uint16_t current_health, uint16_t max_mana,
                               uint16_t current_mana, uint8_t xp_level, uint32_t current_xp_amount,
                               uint32_t max_xp_amount):
        max_health(max_health),
        current_health(current_health),
        max_mana(max_mana),
        current_mana(current_mana),
        xp_level(xp_level),
        current_xp_amount(current_xp_amount),
        max_xp_amount(max_xp_amount) {}
