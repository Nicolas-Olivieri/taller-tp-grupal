#include "inventory_info.h"

#include "common/protocol/serializer.h"


InventoryInfoDTO::InventoryInfoDTO(const std::map<uint8_t, uint8_t, std::greater<>>& items): items(items) {}


size_t InventoryInfoDTO::message_size() const {
    return sizeof(uint16_t) + (sizeof(uint8_t) + sizeof(uint8_t)) * items.size();
}


void InventoryInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
