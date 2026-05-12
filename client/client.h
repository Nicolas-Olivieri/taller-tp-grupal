#ifndef CLIENT_H
#define CLIENT_H

#include "common/socket.h"

#include "client_protocol.h"

class Client {
private:
    ClientProtocol protocol;


public:
    Client(const char* hostname, const char* servname);

    void run();

    // POC base de sdl dado como ejemplo por la cátedra
    int poc_sdl();

    // Animaciones base
    // Codigo MAL (es un main encapsulado en una función)
    int poc_sdl_2();
};

#endif
