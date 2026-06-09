#include "clan_leave_command.h"

#include <cassert>

ClanLeaveCommand::ClanLeaveCommand(const std::string& player_name): player_name(player_name) {}

void ClanLeaveCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(ClanActionPayload(ClanActionType::LEAVE, player_name));
}

void ClanLeaveCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::CLAN, player_name, "Has abandonado el clan")));
            return;

        case ClanActionStatus::IS_FOUNDER:
            error_msg = "Como fundador del clan, no puedes abandonarlo";
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            error_msg = NOT_IN_CLAN_MSG;
            break;
        case ClanActionStatus::IS_MEMBER:
        case ClanActionStatus::NOT_A_PLAYER:
        case ClanActionStatus::NO_RESULT:
        case ClanActionStatus::IS_ALREADY_MEMBER:
        case ClanActionStatus::IS_BANNED_PLAYER:
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
        default:
            throw std::runtime_error("ClanLeaveCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
