#include "chatevent.h"

#include "common/protocol/serializer.h"

ChatEventDTO::ChatEventDTO(const std::string& receiver, const std::string& content):
        EventDTO(CommandType::CHAT), receiver(receiver), content(content) {}

size_t ChatEventDTO::message_size() const {
    return EventDTO::message_size() + sizeof(uint16_t) + receiver.size() + sizeof(uint16_t) + content.size();
}

void ChatEventDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
