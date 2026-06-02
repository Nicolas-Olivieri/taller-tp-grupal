#include "resurrect_command.h"


ResurrectCommand::ResurrectCommand(const std::string& player_name): player_name(player_name) {}


void ResurrectCommand::execute(GameWorld& world) { world.resurrect_player(player_name); }


void ResurrectCommand::build_snapshot(SnapshotBuilder& /* builder */) {
    // TODO:
    // `GameWorld::resurrect_player` debería devolver un resultado de la ejecución de la acción
    // para cargarlo como una acción en la snapshot
}
