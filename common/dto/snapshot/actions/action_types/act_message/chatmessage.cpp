#include "chatmessage.h"

#include "common/protocol/serializer.h"

ChatMessageDTO::ChatMessageDTO(MessageVisibility visibility, const std::string& sender,
                               const std::string& receiver, const std::string& content):
        visibility(visibility), sender(sender), receiver(receiver), content(content) {}

size_t ChatMessageDTO::message_size() const {
    return sizeof(MessageVisibility) + sizeof(uint16_t) + sender.size() + sizeof(uint16_t) + receiver.size() +
           sizeof(uint16_t) + content.size();
}

void ChatMessageDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
