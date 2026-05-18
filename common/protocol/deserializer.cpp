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
            throw std::invalid_argument("Byte de comando no reconocido: " +
                                        byte);
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
            throw std::invalid_argument("Byte de dirección no reconocido: " +
                                        byte);
            // TODO: chequear si es la mejor excepción
    }
}
