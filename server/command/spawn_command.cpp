#include "spawn_command.h"

#include "server/connection/event_broadcaster.h"


SpawnCommand::SpawnCommand(const std::string& player_name):
        player_name(player_name), position(std::nullopt) {}


SpawnCommand::SpawnCommand(const std::string& player_name, const Position& position):
        player_name(player_name), position(position) {}


void SpawnCommand::execute(GameWorld& world) {
    if (position.has_value()) {
        // El jugador reaparece en su última posición conocida
        world.add_player(player_name, position.value());

    } else {
        // El jugador aparece de manera aleatoria
        world.add_player(player_name);
    }
}


void SpawnCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(ActionDTO(AppearanceDTO(0, 0, player_name)));
}
