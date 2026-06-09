#include "clan_join_command.h"

#include <format>

ClanJoinCommand::ClanJoinCommand(const std::string& player_name, const std::string& clan_name):
        player_name(player_name), clan_name(clan_name), result(JoinClanResult::NO_RESULT) {}

void ClanJoinCommand::execute(GameWorld& world) { result = world.join_clan(player_name, clan_name); }

void ClanJoinCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;

    switch (result) {
        case JoinClanResult::SUCCESS:
            builder.add_action(ActionDTO(ChatMessageDTO(
                    MessageType::CLAN, player_name,
                    "Se ha enviado tu solicitud, espera a que el fundador del clan te acepte!")));
            return;

        case JoinClanResult::ALREADY_IN_CLAN:
            error_msg = "Ya perteneces a un clan. Debes abandonarlo para poder solicitar unirte a otro";
            break;
        case JoinClanResult::CLAN_NOT_FOUND:
            error_msg = std::format("No existe un clan llamado \"{}\", prueba con otro nombre", clan_name);
            break;
        case JoinClanResult::NO_RESULT:
        default:
            throw std::runtime_error("ClanJoinCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
