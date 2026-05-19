#include "lobbyhandler.h"

#include <utility>

#include "common/dto/lobby/credentials.h"
#include "common/protocol/protocol.h"

LobbyHandler::LobbyHandler(Socket&& socket,
                           Queue<ConnectionInfo>& waiting_players):
        handshake_finished(false), socket(std::move(socket)), waiting_players(waiting_players) {}

void LobbyHandler::run() {
    Protocol protocol(this->socket);

    CredentialsDTO credentials = protocol.recv_credentials();

    // comprobación de login/regsitro

    // si es registro, recibir personalización del personaje + persistencia

    // envío de información del mundo + snapshot inicial

    move_into_waiting_queue(credentials.username);
}

void LobbyHandler::move_into_waiting_queue(const std::string& username) {
    std::lock_guard<std::mutex> lock(this->mutex);
    
    handshake_finished = true;

    ConnectionInfo info = {username, std::move(this->socket)};
    waiting_players.push(std::move(info));
}

LobbyHandler::~LobbyHandler() {
    stop();
    shutdown_if_blocked();
    join();
}

void LobbyHandler::shutdown_if_blocked() {
    std::lock_guard<std::mutex> lock(this->mutex);

    if (!handshake_finished) {
        this->socket.shutdown(2);
        this->socket.close();
    }
}
