#include "kill_creatures_command.h"
KillCreaturesCommand::KillCreaturesCommand(const std::string& player_name): player_name(player_name) {}

void KillCreaturesCommand::execute(GameWorld& world) { world.cheat_kill_all_creatures(); }

void KillCreaturesCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(
            ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, "Todos los monstruos han muerto")));
}
