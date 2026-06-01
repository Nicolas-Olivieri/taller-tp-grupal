#include "server.h"

#include "common/queue.h"
#include "config/game_config.h"

#define CMD_EXIT 'q'
#define DB_PATH "./playerdata.bin"
#define INDX_PATH "./index.bin"

Server::Server(const char* servname):
        player_repository(DB_PATH, INDX_PATH),
        acceptor(servname, waiting_players, player_repository),
        game_acceptor(waiting_players, player_repository) {
    GameConfig::get();
}

void Server::run() {
    acceptor.start();
    game_acceptor.start();

    // TODO: revisar condición de corte
    while (std::getchar() != CMD_EXIT) {}

    waiting_players.close();
}
