#ifndef _LOBBYHANDLER_H
#define _LOBBYHANDLER_H

#include <mutex>
#include <string>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"
#include "server/persistance/playerrepository.h"

#include "connectioninfo.h"

class LobbyHandler: public Thread {
private:
    std::mutex mutex;
    bool handshake_finished;
    Socket socket;
    Queue<ConnectionInfo>& waiting_players;
    PlayerRepository& player_repository;

    void move_into_waiting_queue(const std::string& username);

    void shutdown_if_blocked();

    // TOOD: de momento es solo para crear jugadores con skins random, después se borra o cambia cómo funciona
    void create_player(const std::string& username);

public:
    LobbyHandler(Socket&& socket, Queue<ConnectionInfo>& waiting_players,
                 PlayerRepository& player_repository);

    void run() override;

    ~LobbyHandler();
};

#endif  // _LOBBYHANDLER_H
