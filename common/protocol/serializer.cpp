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
    serialize(info.safe_gold);
    serialize(info.excess_gold);
    serialize(info.appearance);
    serialize(info.stats);
}

// TODO: se debería serializar dependiendo de action.action (ActionType)
void Serializer::serialize(const ActionDTO& action) {
    serialize(static_cast<uint8_t>(action.action));

    switch (action.action) {
        case ActionType::DESPAWN:
            serialize(action.despawn);
            break;
        case ActionType::MESSAGE:
            serialize(action.chat_message);
            break;
        case ActionType::RESURRECTION:
            serialize(action.resurrection);
            break;
        case ActionType::DEATH:
            serialize(action.death);
            break;
        case ActionType::MESSAGE_LIST:
            serialize(action.list);
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

    copy_to_buffer(value.data(), size);
}

void Serializer::serialize(uint8_t value) { this->buffer[this->offset++] = value; }

void Serializer::serialize(uint16_t value) {
    uint16_t netvalue = ntohs(value);

    copy_to_buffer(&netvalue, sizeof(netvalue));
}

void Serializer::serialize(uint32_t value) {
    uint32_t netvalue = ntohl(value);

    copy_to_buffer(&netvalue, sizeof(netvalue));
}

void Serializer::copy_to_buffer(const void* data, size_t size) {
    std::memcpy(&this->buffer[this->offset], data, size);
    offset += size;
}

void Serializer::serialize(const InteractEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.target_x);
    serialize(event.target_y);
}

void Serializer::serialize(const DespawnDTO& despawn) { serialize(despawn.player_despawned); }

void Serializer::serialize(const AllyInfoDTO& info) {
    serialize(static_cast<uint8_t>(info.type));
    serialize(info.x);
    serialize(info.y);
}

void Serializer::serialize(const ChatMessageDTO& message) {
    serialize(static_cast<uint8_t>(message.visibility));
    serialize(message.sender);
    serialize(message.receiver);
    serialize(message.content);
}

void Serializer::serialize(const ChatEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.receiver);
    serialize(event.content);
}

void Serializer::serialize(const PlayerStatsDTO& stats) {
    serialize(stats.max_health);
    serialize(stats.current_health);
    serialize(stats.max_mana);
    serialize(stats.current_mana);
    serialize(stats.xp_level);
    serialize(stats.current_xp_amount);
    serialize(stats.max_xp_amount);
}

void Serializer::serialize(const ResurrectionDTO& resurrection) {
    serialize(resurrection.player_resurrected);
    serialize(resurrection.original_appearance);
}

void Serializer::serialize(const DeathDTO& death) { serialize(death.player_dead); }

void Serializer::serialize(const ChatListDTO& list) {
    serialize(list.receiver);
    serialize(list.lines);
}
