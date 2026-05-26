#ifndef GAME_ACCEPTOR_H
#define GAME_ACCEPTOR_H

#include <list>
#include <memory>

#include "common/queue.h"
#include "common/thread/thread.h"
#include "server/command/command.h"
#include "server/game/game_loop.h"
#include "server/persistance/playerrepository.h"

#include "client_handler.h"
#include "connectioninfo.h"

class GameAcceptor: public Thread {
private:
    EventBroadcaster broadcaster;

    std::list<ClientHandler> clients;

    Queue<std::unique_ptr<Command>> command_queue;
    Queue<ConnectionInfo>& waiting_queue;
    PlayerRepository& player_repository;

    GameLoop game_loop;

public:
    GameAcceptor(Queue<ConnectionInfo>& waiting_queue, PlayerRepository& player_repository);

    void run() override;

    GameAcceptor(const GameAcceptor&) = delete;

    GameAcceptor& operator=(const GameAcceptor&) = delete;

    ~GameAcceptor() override;

private:
    void reap();
};


#endif  // GAME_ACCEPTOR_H
