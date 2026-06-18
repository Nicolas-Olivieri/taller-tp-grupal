#include "creature_stats.h"

#include "common/protocol/serializer.h"


CreatureStatsDTO::CreatureStatsDTO(const uint16_t max_health, const uint16_t current_health,
                                   const uint8_t xp_level):
        max_health(max_health), current_health(current_health), xp_level(xp_level) {}


size_t CreatureStatsDTO::message_size() const { return 2 * sizeof(uint16_t) + sizeof(uint8_t); }


void CreatureStatsDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
