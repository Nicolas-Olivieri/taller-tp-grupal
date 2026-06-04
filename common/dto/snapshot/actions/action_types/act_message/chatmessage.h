#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <string>

#include "common/dto/message.h"

enum class MessageType : uint8_t { SYSTEM, PRIVATE, GLOBAL, CLAN, ERROR, ALLY };

struct ChatMessageDTO: public ProtocolMessageDTO {
    MessageType type;
    std::string sender;
    std::string receiver;
    std::string content;

    ChatMessageDTO(const MessageType& type, const std::string& sender, const std::string& receiver,
                   const std::string& content);

    ChatMessageDTO(const MessageType& type, const std::string& receiver, const std::string& content);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // CHATMESSAGE_H
