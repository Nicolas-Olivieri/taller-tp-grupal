#include "teleport_info.h"

#include "common/protocol/serializer.h"

TeleportInfoDTO::TeleportInfoDTO(const uint16_t port_a_x, const uint16_t port_a_y,
                                 const uint16_t port_b_x, const uint16_t port_b_y) :
    port_a_x(port_a_x), port_a_y(port_a_y), port_b_x(port_b_x), port_b_y(port_b_y) {}

size_t TeleportInfoDTO::message_size() const {
    return sizeof(uint16_t) * 4;
}

void TeleportInfoDTO::accept(Serializer &serializer) const {
    serializer.serialize(*this);
}
