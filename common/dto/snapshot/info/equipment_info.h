#ifndef EQUIPMENT_INFO_DTO_H
#define EQUIPMENT_INFO_DTO_H

#include "common/dto/message.h"


struct EquipmentInfoDTO: public ProtocolMessageDTO {
    uint8_t weapon;
    uint8_t shield;
    uint8_t helmet;
    uint8_t armor;

    EquipmentInfoDTO();

    EquipmentInfoDTO(uint8_t weapon, uint8_t shield, uint8_t helmet, uint8_t armor);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // EQUIPMENT_INFO_DTO_H
