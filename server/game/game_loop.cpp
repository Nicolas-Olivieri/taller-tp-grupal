#include "game_loop.h"

#include <unordered_map>
#include <vector>

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
            update_world(builder);
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


void GameLoop::update_world(SnapshotBuilder& builder) {
    std::vector<CreatureUpdateStatus> creatures_status = game_world.update();

    // TODO: no sé si esta responsabilidad va acá
    for (const auto& status: creatures_status) {
        if (!status.did_attack)
            continue;
        std::string msg = format_creature_attack_message(status);

        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::SYSTEM, status.player_name, msg)));
    }
}

// TODO: no sé si esta responsabilidad va acá
std::string GameLoop::format_creature_attack_message(const CreatureUpdateStatus& status) {
    // TODO: los númeritos...
    static std::unordered_map<uint8_t, std::string> creature_to_name = {
            {0, "Goblin"}, {1, "Esqueleto"}, {2, "Zombie"}, {3, "Araña"}, {4, "Orco"}, {5, "Golem"}};

    if (status.damage_dealt == 0) {
        return std::format("Esquivaste el ataque de {}!!", creature_to_name.at(status.creature_id));
    } else if (status.killed_target) {
        return std::format("{} te mato", creature_to_name.at(status.creature_id));
    }

    return std::format("{} te quito {} de vida", creature_to_name.at(status.creature_id), status.damage_dealt);
}

void GameLoop::broadcast(SnapshotBuilder& builder) {
    builder.add_players(game_world.get_players());
    builder.add_creatures(game_world.get_creatures());
    broadcaster.broadcast(builder.build());
}


GameLoop::~GameLoop() {
    Thread::stop();
    Thread::join();
}
