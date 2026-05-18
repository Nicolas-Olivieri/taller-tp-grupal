#ifndef CLIENT_H
#define CLIENT_H

#include "SDL/client_game.h"

class Client {
private:
    ClientGame game;

public:
    Client();

    void run();
};

#endif
