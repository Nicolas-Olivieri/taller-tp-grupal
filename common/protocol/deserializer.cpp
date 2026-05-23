#include "deserializer.h"

#include <stdexcept>

#include <arpa/inet.h>

Deserializer::Deserializer(Socket& socket): socket(socket) {}

std::string Deserializer::recv_string() {
    uint16_t size = recv_uint16();

    std::string bytes(size, 0);
    if (size > 0)
        this->socket.recvall(&bytes[0], size);

    return bytes;
}

uint8_t Deserializer::recv_uint8() {
    uint8_t byte;
    this->socket.recvall(&byte, sizeof(byte));

    return byte;
}

uint16_t Deserializer::recv_uint16() {
    uint16_t bytes;
    this->socket.recvall(&bytes, sizeof(bytes));

    return ntohs(bytes);
}

CommandType Deserializer::recv_command_type() {
    uint8_t byte = recv_uint8();

    switch (static_cast<CommandType>(byte)) {
        // TODO: agregar un case para todos los tipos de comandos existentes,
        // luego borrar este comentario
        case CommandType::MOVE:
            return static_cast<CommandType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de comando no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

Direction Deserializer::recv_direction() {
    uint8_t byte = recv_uint8();

    switch (static_cast<Direction>(byte)) {
        // TODO: agregar un case para todos los tipos de comandos existentes,
        // luego borrar este comentario
        case Direction::DOWN:
        case Direction::UP:
        case Direction::LEFT:
        case Direction::RIGHT:
        case Direction::IDLE:
            return static_cast<Direction>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de dirección no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

std::vector<PlayerInfoDTO> Deserializer::recv_players_information() {
    uint16_t size = recv_uint16();

    std::vector<PlayerInfoDTO> players_information;
    players_information.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        players_information.push_back(recv_player_info());
    }

    return players_information;
}

PlayerInfoDTO Deserializer::recv_player_info() {
    std::string name = recv_string();
    Direction direction = recv_direction();
    uint16_t x = recv_uint16();
    uint16_t y = recv_uint16();
    AppearanceDTO appearance = recv_appearance();

    return PlayerInfoDTO(name, direction, x, y, appearance);
}

std::vector<ActionDTO> Deserializer::recv_actions() {
    uint16_t size = recv_uint16();

    std::vector<ActionDTO> actions;
    actions.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        actions.push_back(recv_action());
    }

    return actions;
}

ActionDTO Deserializer::recv_action() {
    ActionType type = recv_action_type();
    // TODO: recordar modificar esto para recibir la información según el
    // ActionType
    if (type == ActionType::PLACEHOLDER)
        return ActionDTO(type);

    throw std::exception();  // TODO: modificar
}

ActionType Deserializer::recv_action_type() {
    uint8_t byte = recv_uint8();

    switch (static_cast<ActionType>(byte)) {
        // TODO: agregar un case para todos los tipos de comandos existentes,
        // luego borrar este comentario
        case ActionType::PLACEHOLDER:
            return static_cast<ActionType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de acción no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

AppearanceDTO Deserializer::recv_appearance() {
    // TODO 1: recordar considerar equipamientos y eso en el futuro
    // TODO 2: capaz en un futuro cada categoría debería ser un enum
    uint8_t body = recv_uint8();
    uint8_t head = recv_uint8();

    return AppearanceDTO(body, head);
}
