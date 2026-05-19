#include "acceptor.h"

#include <utility>

#include "common/liberror.h"

Acceptor::Acceptor(const std::string& servname, Queue<ConnectionInfo>& waiting_players):
        listener(servname.c_str()), waiting_players(waiting_players) {}

void Acceptor::run() {
    while (should_keep_running()) wait_for_player();
}

void Acceptor::wait_for_player() {
    try {
        Socket peer = this->listener.accept();
        reap_lobbies();
        send_to_lobby(std::move(peer));
    } catch (const LibError& socketerr) {
        if (should_keep_running())
            throw;  // vuelve a lanzar la excepción original
    }
}

void Acceptor::reap_lobbies() {
    auto current_lobby = lobbies.begin();

    while (current_lobby != lobbies.end()) {
        if (!(*current_lobby)->is_alive())
            current_lobby = lobbies.erase(current_lobby);
        else
            ++current_lobby;
    }
}

void Acceptor::send_to_lobby(Socket&& peer) {
    std::unique_ptr<LobbyHandler> lobby = std::make_unique<LobbyHandler>(std::move(peer), waiting_players);
    lobby->start();
    this->lobbies.push_back(std::move(lobby));
}

Acceptor::~Acceptor() {
    stop();
    this->listener.shutdown(2);
    this->listener.close();

    join();

    this->lobbies.clear();
}
