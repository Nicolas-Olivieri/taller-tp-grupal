#include "lobbyhandler.h"

#include <random>
#include <utility>

#include "common/dto/lobby/credentials.h"
#include "common/protocol/protocol.h"

LobbyHandler::LobbyHandler(Socket&& socket, Queue<ConnectionInfo>& waiting_players,
                           PlayerRepository& player_repository):
        handshake_finished(false),
        socket(std::move(socket)),
        waiting_players(waiting_players),
        player_repository(player_repository) {}

void LobbyHandler::run() {
    Protocol protocol(this->socket);

    CredentialsDTO credentials = protocol.recv_credentials();

    // comprobación de login/regsitro

    // si es registro, recibir personalización del personaje + persistencia
    if (!player_repository.exists(credentials.username)) {
        try {
            create_player(credentials.username);
        } catch (const PlayerAlreadyExists& errror) {
            // TODO: un jugador se puso el nombre que este quería justo al mismo tiempo y se lo sacó
            std::cout << "[Lobby] " << credentials.username << " is already taken" << std::endl;
            return;
        }
    }

    // envío de información del mundo + snapshot inicial

    move_into_waiting_queue(credentials.username);
}

void LobbyHandler::create_player(const std::string& username) {
    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution get_random_body(0, 2);
    std::uniform_int_distribution get_random_head(0, 3);

    uint8_t body, head;

    body = get_random_body(generator);
    head = get_random_head(generator);

    player_repository.create(username, 0, 0, body, head);
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
