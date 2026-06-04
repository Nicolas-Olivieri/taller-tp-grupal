#include "client.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <SDL2pp/SDLTTF.hh>

#include "SDL/client_game.h"
#include "client/qt/login_window.h"

Client::Client(int argc, char* argv[]): app(argc, argv) {}

int Client::run() {
    lobby.show();

    int error = app.exec();

    if (error)
        return error;

    Socket socket = lobby.get_socket();
    ConnectionHandler connection(std::move(socket));

    SDL2pp::SDLTTF ttf;

    std::string player_name = lobby.get_username();
    ClientGame game(connection, player_name);
    game.run();

    return 0;
}
