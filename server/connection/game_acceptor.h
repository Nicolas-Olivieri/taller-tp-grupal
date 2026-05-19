#ifndef GAME_ACCEPTOR_H
#define GAME_ACCEPTOR_H

#include <list>
#include <memory>

#include "common/queue.h"
#include "common/thread/thread.h"
#include "server/command/command.h"
#include "server/game/game_loop.h"

#include "client_handler.h"
#include "connectioninfo.h"


class GameAcceptor: public Thread {
private:
    std::list<ClientHandler> clients;

    Queue<std::unique_ptr<Command>> command_queue;
    Queue<ConnectionInfo>& waiting_queue;

    GameLoop game_loop;

public:
    explicit GameAcceptor(Queue<ConnectionInfo>& waiting_queue);

    void run() override;

    GameAcceptor(const GameAcceptor&) = delete;

    GameAcceptor& operator=(const GameAcceptor&) = delete;

    ~GameAcceptor() override;

private:
    void reap();
};


#endif  // GAME_ACCEPTOR_H
