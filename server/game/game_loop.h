#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <memory>

#include "common/dto/snapshot/snapshot_builder.h"
#include "common/queue.h"
#include "common/thread/thread.h"
#include "server/command/command.h"

#include "game_world.h"


class GameLoop: public Thread {
private:
    Queue<std::unique_ptr<Command>>& command_queue;
    GameWorld game_world;
    EventBroadcaster& broadcaster;

public:
    explicit GameLoop(Queue<std::unique_ptr<Command>>& command_queue, EventBroadcaster& broadcaster);

    void run() override;

    GameLoop(const GameLoop&) = delete;

    GameLoop& operator=(const GameLoop&) = delete;

    ~GameLoop() override;

private:
    void process_commands(SnapshotBuilder& builder);

    void update_world();

    void broadcast(SnapshotBuilder& builder);
};


#endif  // GAME_LOOP_H
