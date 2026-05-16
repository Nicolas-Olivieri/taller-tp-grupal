#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include "common/dto/credentials.h"
#include "common/socket.h"

class ServerProtocol {
private:
    Socket& socket;

public:
    explicit ServerProtocol(Socket& socket);

    CredentialsDTO recv_credentials();
};


#endif
