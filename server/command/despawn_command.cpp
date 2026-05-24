#include "despawn_command.h"


DespawnCommand::DespawnCommand(const std::string& player_name): player_name(player_name) {}


void DespawnCommand::execute(GameWorld& world) { world.remove_player(player_name); }


void DespawnCommand::build_snapshot(SnapshotBuilder& builder) {
    DespawnDTO despawned(player_name);
    ActionDTO action(despawned);
    builder.add_action(action);
}
