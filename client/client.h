#ifndef CLIENT_H
#define CLIENT_H

#include "client_protocol.h"
#include "common/socket.h"

class Client {
private:
    ClientProtocol protocol;

    int poc_sdl();

public:
    explicit Client(Socket&& skt);

    void run();
};


#endif