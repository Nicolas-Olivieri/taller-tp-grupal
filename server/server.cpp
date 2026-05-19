#include "server.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <toml.hpp>

#include "common/queue.h"

Server::Server(const char* servname): acceptor(servname, waiting_players) {
}

void Server::run() {
    acceptor.start();

    // TODO: revisar condición de corte
    std::string input;
    while (std::cin >> input) {
        if (input.compare("q") == 0)
            break;
    }
}
