#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>

#include "common/dto/credentials.h"
#include "common/socket.h"

class ServerProtocol {
private:
    Socket& socket;

    // TODO: muy posiblemente en el futuro estos métodos privados deban ser
    // movidos a una clase común para que usen los protocolos de ambos lados
    void check_message_byte(const Message& expected);

    std::string recv_string();

    uint16_t recv_uint16();

public:
    explicit ServerProtocol(Socket& socket);  // NOLINT

    CredentialsDTO recv_credentials();
};


#endif
