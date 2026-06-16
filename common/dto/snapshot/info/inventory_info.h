#ifndef INVENTORY_INFO_DTO_H
#define INVENTORY_INFO_DTO_H

#include <map>

#include "common/dto/message.h"


struct InventoryInfoDTO: public ProtocolMessageDTO {
    std::map<uint8_t, uint8_t, std::greater<>> items;

    explicit InventoryInfoDTO(const std::map<uint8_t, uint8_t, std::greater<>>& items);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // INVENTORY_INFO_DTO_H
