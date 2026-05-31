#include "spawn_command.h"

#include "server/connection/event_broadcaster.h"

SpawnCommand::SpawnCommand(const std::string& player_name, const PlayerData& data):
        player_name(player_name), data(data) {}

void SpawnCommand::execute(GameWorld& world) { world.add_player(player_name, data); }

void SpawnCommand::build_snapshot(SnapshotBuilder& /* builder */) {}
