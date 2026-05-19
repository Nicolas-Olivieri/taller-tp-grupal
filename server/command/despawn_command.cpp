#include "despawn_command.h"


DespawnCommand::DespawnCommand(const std::string& player_name):
        player_name(player_name) {}


void DespawnCommand::execute(GameWorld& world) {
    world.remove_player(player_name);
}


void DespawnCommand::broadcast(EventBroadcaster& /* broadcaster */) {}
