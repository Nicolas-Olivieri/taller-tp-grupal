#include "server_protocol.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>


ServerProtocol::ServerProtocol(Socket& socket): socket(socket) {}

CredentialsDTO ServerProtocol::recv_credentials() {
    uint8_t msgbyte;
    this->socket.recvall(&msgbyte, sizeof(msgbyte));
    if (msgbyte != static_cast<uint8_t>(Message::CREDENTIALS))
        throw std::exception();  // TODO: definir excepción

    // TODO: modularizable en métodos para leer distintos tipos de datos
    uint16_t username_size;
    this->socket.recvall(&username_size, sizeof(username_size));
    username_size = ntohs(username_size);

    std::string username(username_size, 0);
    this->socket.recvall(&username[0], username_size);

    uint16_t password_size;
    this->socket.recvall(&password_size, sizeof(password_size));
    password_size = ntohs(password_size);

    std::string password(password_size, 0);
    this->socket.recvall(&password[0], password_size);

    return CredentialsDTO(username, password);
}
