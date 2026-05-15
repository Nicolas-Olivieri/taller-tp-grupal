#ifndef CLIENT_H
#define CLIENT_H

#include "client_protocol.h"
#include "SDL/client_game.h"

class Client {
private:
    ClientGame game;

public:
    Client();

    void run();

};

#endif
