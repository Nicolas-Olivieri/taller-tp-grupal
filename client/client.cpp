#include "client.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "client/qt/lobby.h"

#include "client_game.h"



Client::Client(int argc, char* argv[]): app(argc, argv) {}

int Client::run() {
    lobby.show();

    int error = app.exec();

    if (error)
        return error;

    Socket socket = lobby.get_socket();
    std::string username = lobby.get_username();

    ClientGame game(std::move(socket), username);
    game.run();

    return 0;
}
