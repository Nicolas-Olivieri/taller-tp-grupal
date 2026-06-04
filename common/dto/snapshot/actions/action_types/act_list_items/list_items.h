#ifndef LIST_ITEMS_DTO_H
#define LIST_ITEMS_DTO_H

#include <map>
#include <string>

#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_message/chatmessage.h"

struct ListItemsDTO: public ProtocolMessageDTO {
    MessageType type;
    std::map<uint8_t, uint16_t> items;
    std::string receiver;

    ListItemsDTO() = default;

    ListItemsDTO(const MessageType& type, const std::map<uint8_t, uint16_t>& items,
                 const std::string& receiver);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // LIST_ITEMS_DTO_H
