#ifndef EQUIPABLE_ITEM_INFO_DTO_H
#define EQUIPABLE_ITEM_INFO_DTO_H

#include "common/dto/message.h"


struct EquipableItemInfoDTO: public ProtocolMessageDTO {
    uint8_t item_id;
    uint8_t effect;

    EquipableItemInfoDTO();

    EquipableItemInfoDTO(uint8_t item_id, uint8_t effect);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // EQUIPABLE_ITEM_INFO_DTO_H
