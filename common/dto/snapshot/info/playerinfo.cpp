#include "playerinfo.h"

#include "common/protocol/serializer.h"

PlayerInfoDTO::PlayerInfoDTO(const std::string& name, const Direction& direction, uint16_t x, uint16_t y,
                             uint16_t safe_gold, uint16_t excess_gold, const AppearanceDTO& appearance,
                             const PlayerStatsDTO& stats):
        name(name),
        direction(direction),
        x(x),
        y(y),
        safe_gold(safe_gold),
        excess_gold(excess_gold),
        appearance(appearance),
        stats(stats) {}

size_t PlayerInfoDTO::message_size() const {
    return sizeof(uint16_t) + name.size() + sizeof(direction) + sizeof(x) + sizeof(y) + sizeof(uint16_t) * 2 +
           appearance.message_size() + stats.message_size();
}

void PlayerInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
