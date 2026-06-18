#include "lootinfo.h"

#include "common/protocol/serializer.h"

LootInfoDTO::LootInfoDTO(LootType type, uint16_t x, uint16_t y): type(type), x(x), y(y) {}

size_t LootInfoDTO::message_size() const { return sizeof(type) + sizeof(x) + sizeof(y); }

void LootInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
