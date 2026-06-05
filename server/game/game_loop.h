#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <memory>

#include "common/dto/snapshot/snapshot_builder.h"
#include "common/queue.h"
#include "common/thread/thread.h"
#include "server/command/command.h"
#include "server/persistance/playerrepository.h"

#include "game_world.h"
#include "server/util/server_map_loader.h"

class GameLoop: public Thread {
private:
    ServerMapLoader map_loader;
    Queue<std::unique_ptr<Command>>& command_queue;
    GameWorld game_world;
    EventBroadcaster& broadcaster;
    PlayerRepository& player_repository;

public:
    GameLoop(Queue<std::unique_ptr<Command>>& command_queue, EventBroadcaster& broadcaster,
             PlayerRepository& player_repository);

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
