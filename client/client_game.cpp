#include "client_game.h"

#include <string>
#include <utility>

ClientGame::ClientGame(Socket&& socket, const std::string& username):
        socket(std::move(socket)), username(username) {}

void ClientGame::run() { socket.close(); }
