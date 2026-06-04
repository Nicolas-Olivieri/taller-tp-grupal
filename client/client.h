#ifndef CLIENT_H
#define CLIENT_H

#include <QApplication>
#include <QCoreApplication>

#include "SDL/client_game.h"
#include "common/socket.h"
#include "qt/login_window.h"

class Client {
private:
    QApplication app;
    LoginWindow lobby;

public:
    Client(int argc, char* argv[]);

    int run();
};


#endif  // CLIENT_H
