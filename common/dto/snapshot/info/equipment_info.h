#ifndef EQUIPMENT_INFO_DTO_H
#define EQUIPMENT_INFO_DTO_H

#include "common/dto/message.h"

#include "equipable_item_info.h"


struct EquipmentInfoDTO: public ProtocolMessageDTO {
    EquipableItemInfoDTO weapon;
    EquipableItemInfoDTO shield;
    EquipableItemInfoDTO helmet;
    EquipableItemInfoDTO armor;

    EquipmentInfoDTO();

    EquipmentInfoDTO(const EquipableItemInfoDTO& weapon, const EquipableItemInfoDTO& shield,
                     const EquipableItemInfoDTO& helmet, const EquipableItemInfoDTO& armor);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // EQUIPMENT_INFO_DTO_H
