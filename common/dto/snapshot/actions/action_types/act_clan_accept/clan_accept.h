#ifndef CLAN_JOIN_DTO_H
#define CLAN_JOIN_DTO_H

#include <string>

#include "common/dto/message.h"


struct ClanAcceptDTO: public ProtocolMessageDTO {
    std::string founder;
    std::string accepted;

    ClanAcceptDTO();

    ClanAcceptDTO(const std::string& founder, const std::string& accepted);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_JOIN_DTO_H
