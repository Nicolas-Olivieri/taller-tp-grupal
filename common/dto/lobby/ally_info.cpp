#include "ally_info.h"

#include "common/protocol/serializer.h"


AllyInfoDTO::AllyInfoDTO(const AllyType type, const uint16_t x, const uint16_t y): type(type), x(x), y(y) {}


size_t AllyInfoDTO::message_size() const { return sizeof(type) + sizeof(x) + sizeof(y); }


void AllyInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
