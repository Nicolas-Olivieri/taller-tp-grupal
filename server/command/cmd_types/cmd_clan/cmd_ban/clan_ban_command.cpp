#include "clan_ban_command.h"

#include <cassert>
#include <format>

ClanBanCommand::ClanBanCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanBanCommand::execute(GameWorld& world) {
    result =
            world.execute_clan_action(ClanActionPayload(ClanActionType::BAN, player_name, other_player_name));
}

void ClanBanCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageType::CLAN, player_name,
                                                        std::format("{} fue baneado", other_player_name))));
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::CLAN, other_player_name,
                                             std::format("Fuiste baneado del clan de {}", player_name))));
            return;

        case ClanActionStatus::IS_MEMBER:
            error_msg = IS_MEMBER_MSG;
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            error_msg = NOT_IN_CLAN_MSG;
            break;
        case ClanActionStatus::NOT_A_PLAYER:
            error_msg = std::format(NOT_A_PLAYER_MSG, other_player_name);
            break;
        case ClanActionStatus::NO_RESULT:
        case ClanActionStatus::IS_ALREADY_MEMBER:
        case ClanActionStatus::IS_BANNED_PLAYER:
        case ClanActionStatus::IS_FOUNDER:
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
        default:
            throw std::runtime_error("ClanBanCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
