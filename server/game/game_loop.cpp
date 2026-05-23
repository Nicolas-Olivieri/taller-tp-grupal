#include "game_loop.h"

#include "common/dto/snapshot/snapshot_builder.h"
#include "common/util/rate_timer.h"

#define FPS 30


GameLoop::GameLoop(Queue<std::unique_ptr<Command>>& command_queue, EventBroadcaster& broadcaster):
        command_queue(command_queue), game_world(100, 100), broadcaster(broadcaster) {}


void GameLoop::run() {
    RateTimer timer(FPS);
    int iteration = 0;

    while (should_keep_running()) {
        SnapshotBuilder builder;

        process_commands(builder);
        update_world(iteration);

        broadcast(builder);
        iteration = timer.calculate_next_iteration();
    }
}


void GameLoop::process_commands(SnapshotBuilder& builder) {
    try {
        std::unique_ptr<Command> cmd;
        while (command_queue.try_pop(cmd)) {
            cmd->execute(game_world);
            cmd->build_snapshot(builder);
        }
    } catch (const ClosedQueue&) {}
}


void GameLoop::update_world(const int /* iteration */) {
    // TODO: Implementar lógica
}


void GameLoop::broadcast(SnapshotBuilder& builder) {
    builder.add_players(game_world.get_players());
    broadcaster.broadcast(builder.build());
}


GameLoop::~GameLoop() {
    Thread::stop();
    Thread::join();
}
