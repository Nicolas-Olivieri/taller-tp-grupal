#include "client_protocol.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <utility>

#include <arpa/inet.h>

ClientProtocol::ClientProtocol(Socket& socket): socket(socket) {}

void ClientProtocol::send(const CredentialsDTO& credentials) {
    const Message msgtype = Message::CREDENTIALS;

    uint16_t username_size = static_cast<uint16_t>(credentials.username.size());
    uint16_t password_size = static_cast<uint16_t>(credentials.password.size());

    uint16_t username_netsize = htons(username_size);
    uint16_t password_netsize = htons(password_size);

    size_t buffsize = sizeof(msgtype) + sizeof(username_size) + username_size +
                      sizeof(password_size) + password_size;
    std::vector<uint8_t> buffer(buffsize);

    size_t offset = 0;

    // TODO: modularizable en métodos por cada tipo de dato que quiera escribir
    // (uint16_t y std::string en este caso) y se encargen de mover el offset
    buffer[offset++] = static_cast<uint8_t>(msgtype);

    std::memcpy(&buffer[offset], &username_netsize, sizeof(username_netsize));
    offset += sizeof(username_netsize);

    std::memcpy(&buffer[offset], credentials.username.data(), username_size);
    offset += username_size;

    std::memcpy(&buffer[offset], &password_netsize, sizeof(password_netsize));
    offset += sizeof(password_netsize);

    std::memcpy(&buffer[offset], credentials.password.data(), password_size);

    this->socket.sendall(buffer.data(), buffsize);
}
