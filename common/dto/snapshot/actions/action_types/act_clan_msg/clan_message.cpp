#include "clan_message.h"

#include "common/protocol/serializer.h"

ClanMessageDTO::ClanMessageDTO(const std::string& receiver_clan, const std::string& content,
                               const std::string& sender):
        receiver_clan(receiver_clan), content(content), sender(sender) {}

size_t ClanMessageDTO::message_size() const {
    return sizeof(uint16_t) + receiver_clan.size() + sizeof(uint16_t) + content.size() + sizeof(uint16_t) +
           sender.size();
}

void ClanMessageDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
