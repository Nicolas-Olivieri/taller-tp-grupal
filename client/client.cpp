#include "client.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "SDL/client_game.h"
#include "client/qt/login_window.h"

Client::Client(int argc, char* argv[]): app(argc, argv), lobby(audio_manager) {}

int Client::run() {
    lobby.show();

    int error = app.exec();
    if (error)
        return error;

    if (lobby.was_forced_close()) {
        return 0;
    }

    Socket socket = lobby.get_socket();
    ConnectionHandler connection(std::move(socket));

    std::string player_name = lobby.get_username();
    ClientGame game(connection, player_name, audio_manager, font_manager);
    game.run();

    return 0;
}
