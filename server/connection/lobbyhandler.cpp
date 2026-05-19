#include "lobbyhandler.h"

#include <utility>

#include "common/dto/lobby/credentials.h"
#include "common/protocol/protocol.h"

LobbyHandler::LobbyHandler(Socket&& socket): socket(std::move(socket)) {}

void LobbyHandler::run() {
    Protocol protocol(this->socket);

    CredentialsDTO credentials = protocol.recv_credentials();

    // comprobación de login/regsitro

    // si es registro, recibir personalización del personaje + persistencia

    // envío de información del mundo + snapshot inicial

    // tener la referencia a alguna clase que se encargue de tomar un socket por
    // movimiento y cree el clienthandler
}

LobbyHandler::~LobbyHandler() {
    stop();
    this->socket.shutdown(2);
    this->socket.close();

    join();
}
