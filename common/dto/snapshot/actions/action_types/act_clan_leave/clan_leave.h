#ifndef CLAN_LEAVE_DTO_H
#define CLAN_LEAVE_DTO_H

#include <string>

#include "common/dto/message.h"


struct ClanLeaveDTO: public ProtocolMessageDTO {
    std::string leaver;

    ClanLeaveDTO();

    explicit ClanLeaveDTO(const std::string& leaver);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CLAN_LEAVE_DTO_H
