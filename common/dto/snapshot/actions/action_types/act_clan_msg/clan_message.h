#ifndef CLAN_MESSAGE_DTO_H
#define CLAN_MESSAGE_DTO_H


#include <string>

#include "common/dto/message.h"

struct ClanMessageDTO: public ProtocolMessageDTO {
    std::string receiver_clan;
    std::string content;
    std::string sender;

    ClanMessageDTO();

    ClanMessageDTO(const std::string& receiver_clan, const std::string& content, const std::string& sender);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_MESSAGE_DTO_H
