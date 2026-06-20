#include "game_loop.h"

#include <unordered_map>
#include <vector>

#include "common/dto/snapshot/snapshot_builder.h"
#include "common/util/rate_timer.h"
#include "server/game/creatures/creature_formatter.h"

#include "world_update_status.h"


GameLoop::GameLoop(Queue<std::unique_ptr<Command>>& command_queue, EventBroadcaster& broadcaster,
                   PlayerRepository& player_repository):
        command_queue(command_queue),
        game_world(player_repository),
        broadcaster(broadcaster),
        player_repository(player_repository) {}


void GameLoop::run() {
    game_world.init();

    RateTimer timer(GameConfig::get().get_world_constants().ticks_per_second);
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
        if (current_iteration % GameConfig::get().get_world_constants().tick_between_saves == 0)
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
    const WorldUpdateStatus world_update_status = game_world.update();

    // TODO: no sé si esta responsabilidad va acá
    for (const auto& update: world_update_status.creatures) {
        if (update.status != CreatureStatus::ATTACKED)
            continue;
        broadcast_creature_attack(builder, update);
    }

    broadcast_resurrected_players(builder, world_update_status.resurrected_players);
}

void GameLoop::broadcast_creature_attack(SnapshotBuilder& builder, const CreatureUpdate& update) {
    assert(update.status == CreatureStatus::ATTACKED);
    std::string msg = CreatureFormatter::get_attack_message(update);

    builder.add_action(ActionDTO(AttackDTO("", update.attack.weapon, update.x, update.y,
                                           update.attack.status == AttackStatus::TARGET_DODGED)));
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::SYSTEM, update.attack.player_attacked, msg)));

    // Si mata al objetivo
    if (update.attack.was_killed)
        builder.add_action(ActionDTO(DeathDTO(update.attack.player_attacked)));

    // TODO: creo que nunca falló, pero este assert no puede provocar una race condition en la que el jugador
    // se fue antes de que se haga el broadcast del ataque de la creature? Además, el assert se hace después
    // de haber usado el player_name arriba
    assert(game_world.get_players().contains(update.attack.player_attacked));
    std::string clan_name = game_world.get_players().at(update.attack.player_attacked).get_clan_name();

    if (not clan_name.empty()) {
        std::string clan_msg = CreatureFormatter::get_clan_attack_message(update);
        builder.add_action(ActionDTO(ClanMessageDTO(clan_name, clan_msg, update.attack.player_attacked)));
    }
}

void GameLoop::broadcast_resurrected_players(SnapshotBuilder& builder,
                                             const std::vector<std::string>& resurrected_players) const {
    const auto& players = game_world.get_players();
    for (const auto& player_name: resurrected_players) {
        if (not players.contains(player_name))
            continue;

        const Player& player = players.at(player_name);
        auto appearance = AppearanceDTO(player.get_body(), player.get_head());
        builder.add_action(ActionDTO(ResurrectionDTO(player_name, appearance)));
    }
}


void GameLoop::broadcast(SnapshotBuilder& builder) {
    builder.add_players(game_world.get_players());
    builder.add_creatures(game_world.get_creatures());
    builder.add_loot(game_world.get_lootable_tiles());
    broadcaster.broadcast(builder.build());
}


GameLoop::~GameLoop() {
    Thread::stop();
    Thread::join();
}
