#ifndef SERVER_H
#define SERVER_H

#include "connection/acceptor.h"

class Server {
private:
    Queue<ConnectionInfo> waiting_players;
    Acceptor acceptor;

public:
    explicit Server(const char* servname);

    void run();
};


#endif  // SERVER_H
