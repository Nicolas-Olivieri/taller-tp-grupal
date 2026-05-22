#include "move_command.h"

#include "common/dto/snapshot/snapshot_builder.h"

MoveCommand::MoveCommand(const std::string& player_name, const Direction direction):
        player_name(player_name), direction(direction) {}


void MoveCommand::execute(GameWorld& world) { world.move_player(player_name, direction); }


void MoveCommand::build_snapshot(SnapshotBuilder& /* builder */) {}
