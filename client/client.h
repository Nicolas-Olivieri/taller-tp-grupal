#ifndef CLIENT_H
#define CLIENT_H

#include "SDL/client_game.h"

#include <QApplication>
#include <QCoreApplication>

#include "client/qt/lobby.h"
#include "common/socket.h"

class Client {
private:
    QApplication app;
    Lobby lobby;
    ClientGame game;

public:
    Client(int argc, char* argv[]);

    int run();
};


#endif  // CLIENT_H
