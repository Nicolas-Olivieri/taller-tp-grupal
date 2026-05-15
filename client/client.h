#ifndef CLIENT_H
#define CLIENT_H

#include "common/socket.h"

#include "client_protocol.h"

class Client {

public:
    Client();

    void run();
};

#endif
