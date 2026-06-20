#include "inventory_list.h"

#include "common/protocol/serializer.h"

InventoryListDTO::InventoryListDTO(): inventory({}) {}

InventoryListDTO::InventoryListDTO(const std::string& player_name, const InventoryInfoDTO& inventory):
        player_name(player_name), inventory(inventory) {}

size_t InventoryListDTO::message_size() const {
    return sizeof(uint16_t) + player_name.size() + inventory.message_size();
}

void InventoryListDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
