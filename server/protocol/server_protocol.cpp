#include "server_protocol.h"

#include <algorithm>
#include <utility>
#include <vector>

#include <arpa/inet.h>

ServerProtocol::ServerProtocol(Socket& socket): socket(socket) {}

CredentialsDTO ServerProtocol::recv_credentials() {
    check_message_byte(Message::CREDENTIALS);

    std::string username = recv_string();
    std::string password = recv_string();

    return CredentialsDTO(username, password);
}

void ServerProtocol::check_message_byte(const Message& expected) {
    uint8_t msgbyte;
    this->socket.recvall(&msgbyte, sizeof(msgbyte));

    if (msgbyte != static_cast<uint8_t>(expected))
        throw std::exception();  // TODO: definir excepción
}

std::string ServerProtocol::recv_string() {
    uint16_t size = recv_uint16();

    std::string bytes(size, 0);
    this->socket.recvall(&bytes[0], size);

    return bytes;
}

uint16_t ServerProtocol::recv_uint16() {
    uint16_t bytes;
    this->socket.recvall(&bytes, sizeof(bytes));

    return ntohs(bytes);
}
