#include "inventory_list.h"

#include "common/protocol/serializer.h"

InventoryListDTO::InventoryListDTO(): inventory({}) {}

InventoryListDTO::InventoryListDTO(const InventoryInfoDTO& inventory): inventory(inventory) {}

size_t InventoryListDTO::message_size() const { return inventory.message_size(); }

void InventoryListDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
