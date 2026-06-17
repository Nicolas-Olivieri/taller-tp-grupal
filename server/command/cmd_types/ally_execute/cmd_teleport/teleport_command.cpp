#include "teleport_command.h"

#include <map>

TeleportCommand::TeleportCommand(const std::string& player_name): player_name(player_name), result() {}

void TeleportCommand::execute(GameWorld& world) { result = world.teleport_player(player_name); }

void TeleportCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == TeleportStatus::PLAYER_UNBOUNDED) {
        builder.add_action(ActionDTO(
                ChatMessageDTO(MessageType::ERROR, player_name, "Tenés tocar a un tótem para poder viajar")));
        return;
    }

    static std::map<TeleportStatus, std::string> result_to_message(
            {{TeleportStatus::SUCCESS, "El viaje a través del éter ha sido un éxito..."},
             {TeleportStatus::DESTINATION_BLOCKED,
              "El destino está bloqueado por alguien más, no podés viajar ahora"},
             {TeleportStatus::GHOST_FAIL, "Tu forma espiritual no resistiría el viaje"},
             {TeleportStatus::ACTION_NOT_ACCEPTED, "El tótem no responde a ese llamado"}});

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("TeleportCommand recibió un resultado incorrecto");
    }

    const std::string sender = Ally::ally_type_to_string(result.ally, "TeleportCommand");
    const std::string msg = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, msg)));
}
