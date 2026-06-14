#include "despawn_command.h"


DespawnCommand::DespawnCommand(const std::string& player_name): player_name(player_name), clan_name("") {}


void DespawnCommand::execute(GameWorld& world) {
    if (world.get_players().contains(player_name))
        clan_name = world.get_players().at(player_name).get_clan_name();

    world.remove_player(player_name);
}


void DespawnCommand::build_snapshot(SnapshotBuilder& builder) {
    DespawnDTO despawned(player_name);
    ActionDTO action(despawned);
    builder.add_action(action);

    builder.add_action(
            ActionDTO(ClanMessageDTO(clan_name, std::format("{} se desconectó", player_name), player_name)));
}
