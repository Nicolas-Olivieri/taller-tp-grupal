#include "client.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "SDL/client_game.h"
#include "client/qt/lobby.h"


Client::Client(int argc, char* argv[]): app(argc, argv) {}

int Client::run() {
    lobby.show();

    int error = app.exec();

    if (error)
        return error;

    Socket socket = lobby.get_socket();
    ConnectionHandler connection(std::move(socket));

    ClientGame game(connection);
    game.run();

    return 0;
}
