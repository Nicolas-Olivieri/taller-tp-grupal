#ifndef TELEPORT_INFO_DTO_H
#define TELEPORT_INFO_DTO_H
#include "common/dto/message.h"


struct TeleportInfoDTO: public ProtocolMessageDTO {
    uint16_t port_a_x;
    uint16_t port_a_y;
    uint16_t port_b_x;
    uint16_t port_b_y;

    TeleportInfoDTO(uint16_t port_a_x, uint16_t port_a_y, uint16_t port_b_x, uint16_t port_b_y);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // TELEPORT_INFO_DTO_H
