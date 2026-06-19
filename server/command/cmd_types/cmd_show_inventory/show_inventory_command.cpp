#include "show_inventory_command.h"

#include <map>

ShowInventoryCommand::ShowInventoryCommand(const std::string& player_name):
        player_name(player_name), inventory({}) {}

void ShowInventoryCommand::execute(GameWorld& world) {
    const auto& players = world.get_players();

    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }

    assert(players.contains(player_name));
    const Player& player = it->second;

    inventory = InventoryInfoDTO(player.get_inventory_items());
}

void ShowInventoryCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(ActionDTO(InventoryListDTO(inventory)));
}
