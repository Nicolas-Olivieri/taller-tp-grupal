#ifndef CLIENT_H
#define CLIENT_H

#include <qt6/QtWidgets/QApplication>

#include "client/qt/lobby.h"
#include "common/socket.h"

#include "client_protocol.h"

class Client {
private:
    QApplication app;
    Lobby lobby;


public:
    Client(int argc, char* argv[]);
    // TODO borrar si no se usa
    //    explicit Client(QApplication& app);

    int run();
};

#endif
