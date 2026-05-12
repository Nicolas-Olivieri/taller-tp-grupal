#ifndef SERVER_H
#define SERVER_H
#include "server_protocol.h"
#include "common/socket.h"


class Server {
private:
    ServerProtocol protocol;

    void poc_toml();

public:
    explicit Server(Socket&& skt);

    void run();
};


#endif