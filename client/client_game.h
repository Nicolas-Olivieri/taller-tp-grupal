#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H


#include <string>

#include "common/socket.h"

// Mock Game
class ClientGame {
    Socket socket;
    const std::string username;

public:
    ClientGame(Socket&& socket, const std::string& username);

    void run();
};


#endif  // CLIENT_GAME_H
