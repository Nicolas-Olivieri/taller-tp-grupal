#ifndef INVENTORY_LIST_H
#define INVENTORY_LIST_H

#include <cstdint>
#include <functional>
#include <map>
#include <string>

#include "common/dto/message.h"
#include "common/dto/snapshot/info/inventory_info.h"

struct InventoryListDTO: public ProtocolMessageDTO {
    std::string player_name;
    InventoryInfoDTO inventory;

    InventoryListDTO();

    InventoryListDTO(const std::string& player_name, const InventoryInfoDTO& inventory);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // INVENTORY_LIST_H
