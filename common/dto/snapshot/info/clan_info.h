#ifndef CLAN_INFO_DTO_H
#define CLAN_INFO_DTO_H

#include <string>

#include "common/dto/message.h"


struct ClanInfoDTO: public ProtocolMessageDTO {
    std::string name;
    uint8_t is_founder;  // valor booleano

    ClanInfoDTO(const std::string& name, uint8_t is_founder);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_INFO_DTO_H
