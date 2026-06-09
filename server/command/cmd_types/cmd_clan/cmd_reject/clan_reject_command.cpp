#include "clan_reject_command.h"

#include <cassert>
ClanRejectCommand::ClanRejectCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanRejectCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(
            ClanActionPayload(ClanActionType::ACCEPT, player_name, other_player_name));
}

void ClanRejectCommand::build_snapshot(SnapshotBuilder& builder) {
    assert(false);
    std::string error_msg;
    switch (result.status) {
        case ClanActionStatus::SUCCESS:

            return;

        case ClanActionStatus::IS_FOUNDER:
            break;
        case ClanActionStatus::IS_MEMBER:
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            break;
        case ClanActionStatus::NOT_A_PLAYER:
            break;
        case ClanActionStatus::NO_RESULT:
            break;
        case ClanActionStatus::IS_ALREADY_MEMBER:
            break;
        case ClanActionStatus::IS_BANNED_PLAYER:
            break;
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
            break;
        default:
            throw std::runtime_error("ClanAcceptCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
