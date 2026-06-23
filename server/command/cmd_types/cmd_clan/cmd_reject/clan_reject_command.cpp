#include "clan_reject_command.h"

#include <cassert>
#include <format>
ClanRejectCommand::ClanRejectCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanRejectCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(
            ClanActionPayload(ClanActionType::REJECT, player_name, other_player_name));
}

void ClanRejectCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    const ClanMessagesData& clan_msgs = GameConfig::get().get_clan_messages();

    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            notify_success(builder);
            return;

        case ClanActionStatus::IS_MEMBER:
            error_msg = clan_msgs.is_member_msg;
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            error_msg = clan_msgs.not_in_clan_msg;
            break;
        case ClanActionStatus::NOT_A_PLAYER:
            error_msg = clan_msgs.prefix + other_player_name + clan_msgs.not_a_player_msg;
            break;
        case ClanActionStatus::IS_ALREADY_MEMBER:
            error_msg = clan_msgs.prefix + other_player_name + clan_msgs.is_already_member_msg;
            break;
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
            error_msg = clan_msgs.prefix + other_player_name + clan_msgs.is_not_in_join_list_msg;
            break;
        case ClanActionStatus::PLAYER_DISCONNECTED:
            error_msg = other_player_name + clan_msgs.player_is_disconnected;
            break;
        default:
            throw std::runtime_error("ClanRejectCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}

void ClanRejectCommand::notify_success(SnapshotBuilder& builder) {
    const ClanMessagesData& clan_msgs = GameConfig::get().get_clan_messages();

    builder.add_action(ActionDTO(
            ChatMessageDTO(MessageType::CLAN, player_name,
                           clan_msgs.reject_founder_prefix + other_player_name + clan_msgs.reject_founder)));
    builder.add_action(ActionDTO(
            ChatMessageDTO(MessageType::CLAN, other_player_name, player_name + clan_msgs.request_rejected)));
}
