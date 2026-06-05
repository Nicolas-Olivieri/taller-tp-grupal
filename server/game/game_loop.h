#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <memory>
#include <string>

#include "common/dto/snapshot/snapshot_builder.h"
#include "common/queue.h"
#include "common/thread/thread.h"
#include "server/command/command.h"
#include "server/persistance/playerrepository.h"

#include "game_world.h"
class GameLoop: public Thread {
private:
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

    void update_world(SnapshotBuilder& builder);

    void broadcast(SnapshotBuilder& builder);

    std::string format_creature_attack_message(const CreatureUpdateStatus& status);
};


#endif  // GAME_LOOP_H
