#include "clan_found.h"

#include "common/protocol/serializer.h"


ClanFoundDTO::ClanFoundDTO(): founder("") {}


ClanFoundDTO::ClanFoundDTO(const std::string& founder): founder(founder) {}


size_t ClanFoundDTO::message_size() const { return sizeof(uint16_t) + founder.size(); }


void ClanFoundDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
