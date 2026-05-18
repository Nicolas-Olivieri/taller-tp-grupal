#include "client.h"

#include "SDL/client_game.h"

Client::Client(): game(ClientGame()) {}

void Client::run() {
    // TODO: lanzar los hilos sender/receiver en paralelo al game
    game.run();
}
