#include "chatmessage.h"

#include "common/protocol/serializer.h"

ChatMessageDTO::ChatMessageDTO(const MessageType& type, const std::string& sender,
                               const std::string& receiver, const std::string& content):
        type(type), sender(sender), receiver(receiver), content(content) {}

ChatMessageDTO::ChatMessageDTO(const MessageType& type, const std::string& receiver,
                               const std::string& content):
        type(type), sender(""), receiver(receiver), content(content) {}

size_t ChatMessageDTO::message_size() const {
    return sizeof(MessageType) + sizeof(uint16_t) + sender.size() + sizeof(uint16_t) + receiver.size() +
           sizeof(uint16_t) + content.size();
}

void ChatMessageDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
