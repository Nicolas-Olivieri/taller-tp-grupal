#include "interact_command.h"


InteractCommand::InteractCommand(const std::string& player_name, const int x, const int y):
        player_name(player_name), position(x, y) {}


void InteractCommand::execute(GameWorld& world) { world.interact(player_name, position); }


void InteractCommand::build_snapshot(SnapshotBuilder& /* builder */) {}
