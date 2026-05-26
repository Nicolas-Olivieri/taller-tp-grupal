#ifndef SERVER_H
#define SERVER_H

#include "connection/acceptor.h"
#include "connection/game_acceptor.h"
#include "persistance/playerrepository.h"

class Server {
private:
    Queue<ConnectionInfo> waiting_players;
    PlayerRepository player_repository;
    Acceptor acceptor;
    GameAcceptor game_acceptor;

public:
    explicit Server(const char* servname);

    void run();
};


#endif  // SERVER_H
