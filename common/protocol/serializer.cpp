#include "serializer.h"

#include <cstring>
#include <stdexcept>

#include <arpa/inet.h>

Serializer::Serializer(std::vector<uint8_t>& buffer): buffer(buffer), offset(0) {}

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

void Serializer::serialize(const SnapshotDTO& snapshot) {
    serialize(static_cast<uint8_t>(Message::SNAPSHOT));
    serialize(snapshot.players_information);
    serialize(snapshot.actions);
}

void Serializer::serialize(const PlayerInfoDTO& info) {
    serialize(info.name);
    serialize(static_cast<uint8_t>(info.direction));
    serialize(info.x);
    serialize(info.y);
    serialize(info.appearance);
}

// TODO: se debería serializar dependiendo de action.action (ActionType)
void Serializer::serialize(const ActionDTO& action) {
    serialize(static_cast<uint8_t>(action.action));

    switch (action.action) {
        case ActionType::DESPAWN:
            serialize(action.despawn);
            break;
        default:
            throw std::runtime_error("Serializer encontró un tipo de acción desconocido");
    }
}

void Serializer::serialize(const AppearanceDTO& appearance) {
    serialize(appearance.body);
    serialize(appearance.head);
}

void Serializer::serialize(const std::string& value) {
    uint16_t size = static_cast<uint16_t>(value.size());
    serialize(size);

    std::memcpy(&this->buffer[offset], value.data(), size);
    offset += size;
}

void Serializer::serialize(uint8_t value) { this->buffer[this->offset++] = value; }

void Serializer::serialize(uint16_t value) {
    uint16_t netvalue = ntohs(value);

    std::memcpy(&this->buffer[this->offset], &netvalue, sizeof(netvalue));
    offset += sizeof(netvalue);
}

void Serializer::serialize(const DespawnDTO& despawn) { serialize(despawn.player_despawned); }
