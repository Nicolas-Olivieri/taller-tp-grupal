#include "game_loop.h"

#include "common/dto/snapshot/snapshot_builder.h"
#include "common/util/rate_timer.h"

#define FPS 30
#define SAVE_FRAME (FPS * 10)


GameLoop::GameLoop(Queue<std::unique_ptr<Command>>& command_queue, EventBroadcaster& broadcaster,
                   PlayerRepository& player_repository):
        command_queue(command_queue),
        game_world(39, 76),
        broadcaster(broadcaster),
        player_repository(player_repository) {}


void GameLoop::run() {
    RateTimer timer(FPS);
    int current_iteration = 0;
    int last_iteration = -1;

    while (should_keep_running()) {
        SnapshotBuilder builder;

        process_commands(builder);
        for (int i = 0; i < current_iteration - last_iteration; ++i) {
            update_world();
        }

        broadcast(builder);

        last_iteration = current_iteration;
        current_iteration = timer.calculate_next_iteration();

        // TODO: engrapadísimo también
        if (current_iteration % SAVE_FRAME == 0)
            player_repository.save_progress(game_world.get_players());
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


void GameLoop::update_world() { game_world.update(); }


void GameLoop::broadcast(SnapshotBuilder& builder) {
    builder.add_players(game_world.get_players());
    broadcaster.broadcast(builder.build());
}


GameLoop::~GameLoop() {
    Thread::stop();
    Thread::join();
}
