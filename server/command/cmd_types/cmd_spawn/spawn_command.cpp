#include "spawn_command.h"

#include "server/connection/event_broadcaster.h"

SpawnCommand::SpawnCommand(const std::string& player_name, const PlayerData& data):
        player_name(player_name), data(data), clan_name("") {}

void SpawnCommand::execute(GameWorld& world) {
    world.add_player(player_name, data);

    assert(world.get_players().contains(player_name));
    const std::string player_clan_name = world.get_players().at(player_name).get_clan_name();

    if (not player_clan_name.empty())
        clan_name = player_clan_name;
}

void SpawnCommand::build_snapshot(SnapshotBuilder& builder) {
    if (clan_name.empty())
        return;

    builder.add_action(
            ActionDTO(ClanMessageDTO(clan_name, std::format("{} se conectó", player_name), player_name)));
}
