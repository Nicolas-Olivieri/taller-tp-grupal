#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include <string>
#include <vector>

#include "common/dto/credentials.h"
#include "common/socket.h"

class ClientProtocol {
private:
    Socket& socket;

    // TODO: muy posiblemente en el futuro estos métodos privados deban ser
    // movidos a una clase común para que usen los protocolos de ambos lados
    void copy_string(std::vector<uint8_t>& buffer, size_t& offset,  // NOLINT
                     const std::string& value);

    void copy_uint16(std::vector<uint8_t>& buffer, size_t& offset,  // NOLINT
                     uint16_t value);

public:
    explicit ClientProtocol(Socket& socket);  // NOLINT

    void send(const CredentialsDTO& credentials);
};


#endif  // CLIENTPROTOCOL_H
