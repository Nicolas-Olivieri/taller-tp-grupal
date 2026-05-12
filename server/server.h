#ifndef SERVER_H
#define SERVER_H
#include <string>

#include "common/socket.h"

#include "server_protocol.h"

class Server {
private:
    ServerProtocol protocol;


public:
    explicit Server(const char* servname);

    void run();

    void poc_toml();
};


#endif
