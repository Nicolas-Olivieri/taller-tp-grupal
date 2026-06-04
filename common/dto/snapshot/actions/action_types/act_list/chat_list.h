#ifndef CHAT_LIST_H
#define CHAT_LIST_H

#include <string>
#include <vector>

#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_message/chatmessage.h"

// es una lista de mensajes que le llegan a 1 solo usuario (siempre privada)
struct ChatListDTO: public ProtocolMessageDTO {
    MessageType type;
    std::vector<std::string> lines;
    std::string receiver;

    ChatListDTO(const MessageType& type, const std::vector<std::string>& lines, const std::string& receiver);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CHAT_LIST_H
