#ifndef INVENTORY_INFO_DTO_H
#define INVENTORY_INFO_DTO_H

#include <unordered_map>

#include "common/dto/message.h"


struct InventoryInfoDTO: public ProtocolMessageDTO {
    std::unordered_map<uint8_t, uint8_t> items;

    explicit InventoryInfoDTO(const std::unordered_map<uint8_t, uint8_t>& items);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // INVENTORY_INFO_DTO_H
