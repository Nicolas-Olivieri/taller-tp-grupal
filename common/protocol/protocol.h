#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common/dto/command.h"
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

    // TODO: no implementa ProtocolMessageDTO, por lo que la idea del factory
    // seguramente no se implemente
    RequestedCommandDTO recv_command();
};

#endif  // PROTOCOL_H
