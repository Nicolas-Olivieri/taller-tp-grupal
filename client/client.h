#ifndef CLIENT_H
#define CLIENT_H

#include <QApplication>
#include <QCoreApplication>

#include "SDL/client_game.h"
#include "client/qt/lobbywindow.h"
#include "common/socket.h"

class Client {
private:
    QApplication app;
    LobbyWindow lobby;

public:
    Client(int argc, char* argv[]);

    int run();
};


#endif  // CLIENT_H
