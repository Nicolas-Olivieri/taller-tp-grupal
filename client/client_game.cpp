#include "client_game.h"

#include <utility>

ClientGame::ClientGame(Socket&& socket): socket(std::move(socket)) {}

void ClientGame::run() { socket.close(); }
