#include "protocol.h"

#include <stdexcept>
#include <string>
#include <vector>

#include "deserializer.h"
#include "serializer.h"

Protocol::Protocol(Socket& socket): socket(socket) {}

void Protocol::send(const ProtocolMessageDTO& dto) {
    size_t buffsize = sizeof(Message) + dto.message_size();
    std::vector<uint8_t> buffer(buffsize);

    Serializer serializer(buffer);
    serializer.serialize(dto);

    this->socket.sendall(buffer.data(), buffsize);
}

CredentialsDTO Protocol::recv_credentials() {
    check_header_message_byte(Message::CREDENTIALS);

    Deserializer deserializer(this->socket);

    std::string username = deserializer.recv_string();
    std::string password = deserializer.recv_string();

    return CredentialsDTO(username, password);
}

ExistenceDTO Protocol::recv_existence() {
    check_header_message_byte(Message::EXISTENCE);

    Deserializer deserializer(this->socket);

    const uint8_t exists = deserializer.recv_uint8();

    const uint8_t connected = deserializer.recv_uint8();

    return ExistenceDTO(exists, connected);
}

ClientMapDataDTO Protocol::recv_map() {
    check_header_message_byte(Message::MAP);

    Deserializer deserializer(this->socket);

    const uint16_t width = deserializer.recv_uint16();
    const uint16_t height = deserializer.recv_uint16();

    const std::vector<AssetInfoDTO> tiles = deserializer.recv_assets_information();
    const std::vector<AssetInfoDTO> colliders = deserializer.recv_assets_information();
    const std::vector<AssetInfoDTO> npcs = deserializer.recv_assets_information();

    return ClientMapDataDTO(width, height, tiles, colliders, npcs);
}

CreatePlayerDTO Protocol::recv_appearance() {
    check_header_message_byte(Message::CREATE_PLAYER);

    Deserializer deserializer(this->socket);

    const uint8_t body = deserializer.recv_uint8();
    const uint8_t head = deserializer.recv_uint8();
    const uint8_t archetype = deserializer.recv_uint8();
    const uint8_t race = deserializer.recv_uint8();

    return CreatePlayerDTO(body, head, archetype, race);
}

RequestedCommandDTO Protocol::recv_command() {
    check_header_message_byte(Message::COMMAND);

    Deserializer deserializer(this->socket);

    CommandType command = deserializer.recv_command_type();

    if (command == CommandType::INTERACT) {
        const int x = deserializer.recv_uint16();
        const int y = deserializer.recv_uint16();

        return RequestedCommandDTO(command, x, y);
    } else if (command == CommandType::MOVE) {
        Direction direction = deserializer.recv_direction();

        return RequestedCommandDTO(command, direction);
    } else if (command == CommandType::CHAT) {
        const std::string receiver = deserializer.recv_string();
        const std::string content = deserializer.recv_string();

        return RequestedCommandDTO(command, receiver, content);
    } else if (command == CommandType::RESURRECT or command == CommandType::HEAL or
               command == CommandType::LIST_ITEMS or command == CommandType::CLAN_REVIEW or
               command == CommandType::CLAN_LEAVE or command == CommandType::PICKUP) {
        return RequestedCommandDTO(command);
    } else if (command == CommandType::BUY_ITEM or command == CommandType::SELL_ITEM or
               command == CommandType::DEPOSIT_ITEM or command == CommandType::WITHDRAW_ITEM or
               command == CommandType::USE_ITEM or command == CommandType::DROP_ITEM or
               command == CommandType::UNEQUIP_ITEM) {
        const uint8_t item_id = deserializer.recv_uint8();
        return RequestedCommandDTO(command, item_id);
    } else if (command == CommandType::DEPOSIT_GOLD or command == CommandType::WITHDRAW_GOLD) {
        const uint16_t gold_amount = deserializer.recv_uint16();
        return RequestedCommandDTO(command, gold_amount);
    } else if (command == CommandType::CLAN_FOUND or command == CommandType::CLAN_JOIN) {
        const std::string clan_name = deserializer.recv_string();
        return RequestedCommandDTO(command, clan_name);
    } else if (command == CommandType::CLAN_REQUEST_RESPONSE) {
        const std::string player_name = deserializer.recv_string();
        const bool is_accepted = deserializer.recv_uint8();
        return RequestedCommandDTO(command, player_name, is_accepted);
    } else if (command == CommandType::CLAN_REMOVE_PLAYER) {
        const std::string player_name = deserializer.recv_string();
        const bool is_permanent_removal = deserializer.recv_uint8();
        return RequestedCommandDTO(command, player_name, is_permanent_removal);
    } else if (command == CommandType::CHEAT_XP) {
        const uint8_t level = deserializer.recv_uint8();
        return RequestedCommandDTO(command, level);
    } else if (command == CommandType::CHEAT_GOLD) {
        const uint16_t amount = deserializer.recv_uint16();
        return RequestedCommandDTO(command, amount);
    } else {
        throw std::invalid_argument("The received command type has no known way to be deserialized");
    }

    /* TODO 2: agregar los siguientes al implementar los correspondientes
    comandos item_id
    */
}

SnapshotDTO Protocol::recv_snapshot() {
    check_header_message_byte(Message::SNAPSHOT);

    Deserializer deserializer(this->socket);

    std::vector<PlayerInfoDTO> players_information = deserializer.recv_players_information();
    std::vector<CreatureInfoDTO> creatures_information = deserializer.recv_creatures_information();
    std::vector<LootInfoDTO> loot_information = deserializer.recv_loot_information();
    std::vector<ActionDTO> actions = deserializer.recv_actions();

    return SnapshotDTO(players_information, creatures_information, loot_information, actions);
}

void Protocol::check_header_message_byte(const Message& expected) {
    Deserializer deserializer(this->socket);
    uint8_t msgbyte = deserializer.recv_uint8();

    if (msgbyte != static_cast<uint8_t>(expected)) {
        throw std::runtime_error("Se recibió un byte que no era el esperado en "
                                 "el protocolo");  // TODO: definir excepción
    }
}
