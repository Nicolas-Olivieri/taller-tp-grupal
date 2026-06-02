#include "heal_command.h"


HealCommand::HealCommand(const std::string& player_name): player_name(player_name) {}


void HealCommand::execute(GameWorld& world) { world.heal_player(player_name); }


void HealCommand::build_snapshot(SnapshotBuilder& /* builder */) {
    // TODO:
    // `GameWorld::heal_player` debería devolver un resultado de la ejecución de la acción
    // para cargarlo como una acción en la snapshot
}
