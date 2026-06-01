#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <string>

#include "common/dto/message.h"

enum class MessageVisibility : uint8_t { PRIVATE, CLAN };

struct ChatMessageDTO: public ProtocolMessageDTO {
    MessageVisibility visibility;
    std::string receiver;
    std::string content;

    ChatMessageDTO(MessageVisibility visibility, const std::string& receiver, const std::string& content);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // CHATMESSAGE_H
