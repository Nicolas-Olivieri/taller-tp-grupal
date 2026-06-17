#include "teleport_command.h"

TeleportCommand::TeleportCommand(const std::string& player_name): player_name(player_name), result() {}

void TeleportCommand::execute(GameWorld& world) { result = world.teleport_player(player_name); }

void TeleportCommand::build_snapshot(SnapshotBuilder& builder) {

    assert(false);  // TODO agregar el build action correcto dependiendo del resultado

    // TODO cambiar el tipo de aliado al del result
    const std::string sender = Ally::ally_type_to_string(AllyType::TOTEM, "TeleportCommand");
    builder.add_action(ActionDTO(
            ChatMessageDTO(MessageType::ALLY, sender, player_name, "Te has teletransportado al Destino")));
}
