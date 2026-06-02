
#include "player_stats.h"

#include "common/protocol/serializer.h"

size_t PlayerStatsDTO::message_size() const { return sizeof(uint16_t) * 2 + sizeof(uint16_t) * 2; }

void PlayerStatsDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }

PlayerStatsDTO::PlayerStatsDTO(uint16_t max_health, uint16_t current_health, uint16_t max_mana,
                               uint16_t current_mana):
        max_health(max_health),
        current_health(current_health),
        max_mana(max_mana),
        current_mana(current_mana) {}
