#include "clan_reject_command.h"

#include <cassert>
#include <format>
ClanRejectCommand::ClanRejectCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanRejectCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(
            ClanActionPayload(ClanActionType::ACCEPT, player_name, other_player_name));
}

void ClanRejectCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            notify_success(builder);
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
        case ClanActionStatus::IS_ALREADY_MEMBER:
            error_msg = std::format(IS_ALREADY_MEMBER_MSG, other_player_name);
            break;
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
            error_msg = std::format(IS_NOT_IN_JOIN_LIST_MSG, other_player_name);
            break;
        case ClanActionStatus::IS_FOUNDER:
        case ClanActionStatus::IS_BANNED_PLAYER:
        case ClanActionStatus::NO_RESULT:
        default:
            throw std::runtime_error("ClanRejectCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}

void ClanRejectCommand::notify_success(SnapshotBuilder& builder) {
    builder.add_action(ActionDTO(
            ChatMessageDTO(MessageType::CLAN, player_name,
                           std::format("Se rechazo correctamente la solicitud de {}", other_player_name))));
    builder.add_action(ActionDTO(
            ChatMessageDTO(MessageType::CLAN, other_player_name,
                           std::format("{} rechazo tu solicitud de unirte a su clan", player_name))));
}
