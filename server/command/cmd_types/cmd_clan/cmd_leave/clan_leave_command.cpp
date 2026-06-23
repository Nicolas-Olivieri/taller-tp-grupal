#include "clan_leave_command.h"

#include <cassert>

#include "common/dto/snapshot/actions/action_types/act_clan_leave/clan_leave.h"

ClanLeaveCommand::ClanLeaveCommand(const std::string& player_name): player_name(player_name) {}

void ClanLeaveCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(ClanActionPayload(ClanActionType::LEAVE, player_name));
}

void ClanLeaveCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    const ClanMessagesData& clan_msgs = GameConfig::get().get_clan_messages();

    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            builder.add_action(ActionDTO(ClanLeaveDTO(player_name)));
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::CLAN, player_name, clan_msgs.player_left_clan)));
            return;

        case ClanActionStatus::IS_FOUNDER:
            error_msg = clan_msgs.founder_cannot_leave_clan;
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            error_msg = clan_msgs.not_in_clan_msg;
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
