#include "clan_accept.h"

#include "common/protocol/serializer.h"


ClanAcceptDTO::ClanAcceptDTO(): founder(""), accepted("") {}


ClanAcceptDTO::ClanAcceptDTO(const std::string& founder, const std::string& accepted):
        founder(founder), accepted(accepted) {}


size_t ClanAcceptDTO::message_size() const {
    return sizeof(uint16_t) + founder.size() + sizeof(uint16_t) + accepted.size();
}


void ClanAcceptDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
