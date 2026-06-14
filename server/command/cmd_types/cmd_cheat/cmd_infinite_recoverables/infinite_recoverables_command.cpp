#include "infinite_recoverables_command.h"

InfiniteRecoberableStatsCommand::InfiniteRecoberableStatsCommand(const std::string& player_name): player_name(player_name) {

}

void InfiniteRecoberableStatsCommand::execute(GameWorld& world) {
    world.cheat_infinite_recoverables(player_name);
}

void InfiniteRecoberableStatsCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, "Se usó el cheat de vida y maná infinita")));
}
