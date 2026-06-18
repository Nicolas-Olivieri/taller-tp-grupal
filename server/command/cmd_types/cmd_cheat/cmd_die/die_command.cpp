#include "die_command.h"

DieCommand::DieCommand(const std::string& player_name): player_name(player_name) {}

void DieCommand::execute(GameWorld& world) { world.cheat_kill_player(player_name); }

void DieCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(
            ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, "Te convertiste en fantasma")));
    builder.add_action(ActionDTO(DeathDTO(player_name)));
}
