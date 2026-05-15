#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H


#include "common/socket.h"

// Mock Game
class ClientGame {
    Socket socket;

public:
    explicit ClientGame(Socket&& socket);

    void run();
};


#endif  // CLIENT_GAME_H
