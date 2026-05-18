#include "playerinfo.h"

#include "common/protocol/serializer.h"

PlayerInfoDTO::PlayerInfoDTO(const std::string& name,
                             const Direction& direction, uint16_t x,
                             uint16_t y):
        name(name), direction(direction), x(x), y(y) {}

size_t PlayerInfoDTO::message_size() const {
    return sizeof(uint16_t) + name.size() + sizeof(direction) + sizeof(x) +
           sizeof(y);
}

void PlayerInfoDTO::accept(Serializer& serializer) const {
    serializer.serialize(*this);
}
