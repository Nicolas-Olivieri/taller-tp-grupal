#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common/dto/credentials.h"
#include "common/dto/message.h"
#include "common/socket.h"

class Protocol {
private:
    Socket& socket;

    void check_header_message_byte(const Message& expected);

public:
    explicit Protocol(Socket& socket);  // NOLINT

    void send(const ProtocolMessageDTO& dto);

    // TODO: en un futuro podría considerarse cambiar cada recv_x por un recv
    // único + patrón factory de DTOs
    CredentialsDTO recv_credentials();
};

#endif  // PROTOCOL_H
