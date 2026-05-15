#ifndef SERVER_H
#define SERVER_H
#include <string>

#include "common/socket.h"

#include "server_protocol.h"

class Server {
public:
    Server();

    void run();
};


#endif
