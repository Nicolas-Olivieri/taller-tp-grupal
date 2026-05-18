#include "serializer.h"

#include <cstring>

#include <arpa/inet.h>

Serializer::Serializer(std::vector<uint8_t>& buffer):
        buffer(buffer), offset(0) {}

// Double dispatch para definir cómo se serializa cada DTO
void Serializer::serialize(const ProtocolMessageDTO& dto) { dto.accept(*this); }

void Serializer::serialize(const CredentialsDTO& credentials) {
    serialize(static_cast<uint8_t>(Message::CREDENTIALS));
    serialize(credentials.username);
    serialize(credentials.password);
}

void Serializer::serialize(const EventDTO& event) {
    serialize(static_cast<uint8_t>(Message::COMMAND));
    serialize(static_cast<uint8_t>(event.command));
}

void Serializer::serialize(const MoveEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(static_cast<uint8_t>(event.direction));
}

void Serializer::serialize(const std::string& value) {
    uint16_t size = static_cast<uint16_t>(value.size());
    uint16_t netsize = htons(size);

    serialize(netsize);

    std::memcpy(&this->buffer[offset], value.data(), size);
    offset += size;
}

void Serializer::serialize(uint8_t value) {
    this->buffer[this->offset++] = value;
}

void Serializer::serialize(uint16_t value) {
    std::memcpy(&this->buffer[this->offset], &value, sizeof(value));
    offset += sizeof(value);
}
