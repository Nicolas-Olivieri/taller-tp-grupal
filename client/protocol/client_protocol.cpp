#include "client_protocol.h"

#include <algorithm>
#include <cstring>
#include <utility>

#include <arpa/inet.h>

ClientProtocol::ClientProtocol(Socket& socket): socket(socket) {}

void ClientProtocol::send(const CredentialsDTO& credentials) {
    Message msgtype = Message::CREDENTIALS;

    size_t buffsize = sizeof(msgtype) + sizeof(uint16_t) +
                      credentials.username.size() + sizeof(uint16_t) +
                      credentials.password.size();

    std::vector<uint8_t> buffer(buffsize);
    size_t offset = 0;

    buffer[offset++] = static_cast<uint8_t>(msgtype);
    copy_string(buffer, offset, credentials.username);
    copy_string(buffer, offset, credentials.password);

    this->socket.sendall(buffer.data(), buffsize);
}

void ClientProtocol::copy_string(std::vector<uint8_t>& buffer, size_t& offset,
                                 const std::string& value) {
    uint16_t size = static_cast<uint16_t>(value.size());
    uint16_t netsize = htons(size);

    copy_uint16(buffer, offset, netsize);

    std::memcpy(&buffer[offset], value.data(), size);
    offset += size;
}

void ClientProtocol::copy_uint16(std::vector<uint8_t>& buffer, size_t& offset,
                                 uint16_t value) {
    std::memcpy(&buffer[offset], &value, sizeof(value));
    offset += sizeof(value);
}
