#ifndef LIST_BANK_DTO_H
#define LIST_BANK_DTO_H

#include <cstdint>
#include <map>
#include <string>

#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_message/chatmessage.h"


struct ListBankDTO: public ProtocolMessageDTO {
    MessageType type;
    uint16_t gold;
    std::map<uint8_t, uint16_t> items;
    std::string receiver;

    ListBankDTO();

    ListBankDTO(const MessageType& type, uint16_t gold, const std::map<uint8_t, uint16_t>& items,
                const std::string& receiver);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // LIST_BANK_DTO_H
