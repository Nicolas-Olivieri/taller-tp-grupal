#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include <vector>

#include "common/dto/credentials.h"
#include "common/socket.h"

class ClientProtocol {
private:
    Socket& socket;

public:
    explicit ClientProtocol(Socket& socket);

    void send(const CredentialsDTO& credentials);
};


#endif  // CLIENTPROTOCOL_H
