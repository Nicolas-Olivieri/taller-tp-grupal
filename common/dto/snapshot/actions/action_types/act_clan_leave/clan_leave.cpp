#include "clan_leave.h"

#include "common/protocol/serializer.h"


ClanLeaveDTO::ClanLeaveDTO(): leaver("") {}


ClanLeaveDTO::ClanLeaveDTO(const std::string& leaver): leaver(leaver) {}


size_t ClanLeaveDTO::message_size() const { return sizeof(uint16_t) + leaver.size(); }


void ClanLeaveDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
