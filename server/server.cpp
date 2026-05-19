#include "server.h"

#include "common/queue.h"

#define CMD_EXIT 'q'


Server::Server(const char* servname):
        acceptor(servname, waiting_players), game_acceptor(waiting_players) {}

void Server::run() {
    acceptor.start();
    game_acceptor.start();

    // TODO: revisar condición de corte
    while (std::getchar() != CMD_EXIT) {}

    waiting_players.close();
}
