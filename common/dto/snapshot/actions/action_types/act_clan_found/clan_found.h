#ifndef CLAN_FOUND_DTO_H
#define CLAN_FOUND_DTO_H

#include <string>

#include "common/dto/message.h"


struct ClanFoundDTO: public ProtocolMessageDTO {
    std::string founder;

    ClanFoundDTO();

    explicit ClanFoundDTO(const std::string& founder);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_FOUND_DTO_H
