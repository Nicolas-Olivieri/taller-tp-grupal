#include "lobbyhandler.h"

#include <random>
#include <utility>

#include "common/dto/lobby/credentials.h"
#include "common/dto/lobby/existence.h"
#include "common/dto/snapshot/map/client_map_data.h"
#include "common/protocol/protocol.h"
#include "server/util/server_map_loader.h"

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
            protocol.send(ExistenceDTO(false, false));
            CreatePlayerDTO appearance = protocol.recv_appearance();
            create_player(credentials.username, appearance);

        } catch (const PlayerAlreadyExists& errror) {
            // TODO: un jugador se puso el nombre que este quería justo al mismo tiempo y se lo sacó
            std::cout << "[Lobby] " << credentials.username << " is already taken" << std::endl;
            return;
        } catch (const ClosedSocket& errror) {
            return;
        }
    } else {
        if (player_repository.is_connected(credentials.username)) {
            protocol.send(ExistenceDTO(true, true));
            return;
        }
        protocol.send(ExistenceDTO(true, false));
    }

    // envío de información del mundo + snapshot inicial
    ServerMapLoader loader;
    protocol.send(loader.get_client_data());

    move_into_waiting_queue(credentials.username);
}

void LobbyHandler::create_player(const std::string& username, const CreatePlayerDTO& player_data) {
    auto [appearance, archetype, race] = player_data;
    auto [body, head] = appearance;
    player_repository.create(username, archetype, race, body, head);
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
