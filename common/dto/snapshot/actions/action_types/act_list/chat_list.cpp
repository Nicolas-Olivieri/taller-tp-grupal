#include "chat_list.h"

#include <numeric>

#include "common/protocol/serializer.h"

ChatListDTO::ChatListDTO(const std::vector<std::string>& lines, const std::string& receiver):
        lines(lines), receiver(receiver) {}

ChatListDTO::ChatListDTO(const std::string& line, const std::string& receiver):
        lines({line}), receiver(receiver) {}

size_t ChatListDTO::message_size() const {
    return sizeof(uint16_t) + receiver.size() + sizeof(uint16_t) +
           std::accumulate(lines.begin(), lines.end(), 0, [](size_t accumulated, const auto& line) {
               return accumulated + sizeof(uint16_t) + line.size();
           });
}

void ChatListDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
