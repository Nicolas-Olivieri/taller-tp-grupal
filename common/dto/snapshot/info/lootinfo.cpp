#include "lootinfo.h"

#include "common/protocol/serializer.h"

LootInfoDTO::LootInfoDTO(bool is_item, uint16_t x, uint16_t y): is_item(is_item), x(x), y(y) {}

size_t LootInfoDTO::message_size() const { return sizeof(is_item) + sizeof(x) + sizeof(y); }

void LootInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
