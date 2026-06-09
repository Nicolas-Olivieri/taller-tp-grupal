#include "clan_kick_command.h"

#include <cassert>
#include <format>

ClanKickCommand::ClanKickCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanKickCommand::execute(GameWorld& world) {
    result =
            world.execute_clan_action(ClanActionPayload(ClanActionType::BAN, player_name, other_player_name));
}

void ClanKickCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::CLAN, player_name,
                                             std::format("{} fue expulsado del clan", other_player_name))));
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::CLAN, other_player_name,
                                             std::format("Fuiste expulsado del clan por {}", player_name))));
            return;

        case ClanActionStatus::IS_FOUNDER:
            error_msg = "No puedes expulsarte a tí mismo del clan";
            break;
        case ClanActionStatus::IS_MEMBER:
            error_msg = IS_MEMBER_MSG;
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            error_msg = std::format("El jugador {} no es un miembro del clan", other_player_name);
            break;
        case ClanActionStatus::NOT_A_PLAYER:
            error_msg = std::format(NOT_A_PLAYER_MSG, other_player_name);
            break;
        case ClanActionStatus::IS_ALREADY_MEMBER:
        case ClanActionStatus::IS_BANNED_PLAYER:
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
        case ClanActionStatus::NO_RESULT:
        default:
            throw std::runtime_error("ClanKickCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
