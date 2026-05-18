#include "protocol.h"

#include <string>
#include <vector>

#include "deserializer.h"
#include "serializer.h"

Protocol::Protocol(Socket& socket): socket(socket) {}

void Protocol::send(const ProtocolMessageDTO& dto) {
    size_t buffsize = dto.message_size();
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

void Protocol::check_header_message_byte(const Message& expected) {
    Deserializer deserializer(this->socket);
    uint8_t msgbyte = deserializer.recv_uint8();

    if (msgbyte != static_cast<uint8_t>(expected))
        throw std::exception();  // TODO: definir excepción
}
