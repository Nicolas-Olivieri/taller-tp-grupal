#include "clan_info.h"

#include "common/protocol/serializer.h"


ClanInfoDTO::ClanInfoDTO(const std::string& name, const uint8_t is_founder):
        name(name), is_founder(is_founder) {}


size_t ClanInfoDTO::message_size() const { return sizeof(uint16_t) + name.size() + sizeof(uint8_t); }


void ClanInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
