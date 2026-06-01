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
    } else {
        throw std::invalid_argument("The received command type has no known way to be deserialized");
    }

    /* TODO 2: agregar los siguientes al implementar los correspondientes
    comandos item_id clan_name
    */
}

SnapshotDTO Protocol::recv_snapshot() {
    check_header_message_byte(Message::SNAPSHOT);

    Deserializer deserializer(this->socket);

    std::vector<PlayerInfoDTO> players_information = deserializer.recv_players_information();
    // TODO: std::vector<CreatureDTO> creatures_information =
    // deserializer.recv_creatures_information();
    std::vector<ActionDTO> actions = deserializer.recv_actions();

    return SnapshotDTO(players_information, actions);
}

void Protocol::check_header_message_byte(const Message& expected) {
    Deserializer deserializer(this->socket);
    uint8_t msgbyte = deserializer.recv_uint8();

    if (msgbyte != static_cast<uint8_t>(expected)) {
        throw std::runtime_error("Se recibió un byte que no era el esperado en "
                                 "el protocolo");  // TODO: definir excepción
    }
}
