#include "asset_info.h"

#include "common/protocol/serializer.h"

AssetInfoDTO::AssetInfoDTO(const uint8_t id, const uint16_t x, const uint16_t y): id(id), x(x), y(y) {}

size_t AssetInfoDTO::message_size() const { return sizeof(id) + sizeof(x) + sizeof(y); }

void AssetInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
