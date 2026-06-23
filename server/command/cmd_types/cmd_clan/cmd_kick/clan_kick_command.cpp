#include "clan_kick_command.h"

#include <cassert>
#include <format>

ClanKickCommand::ClanKickCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanKickCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(
            ClanActionPayload(ClanActionType::KICK, player_name, other_player_name));
}

void ClanKickCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    const ClanMessagesData& clan_msgs = GameConfig::get().get_clan_messages();

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
            error_msg = clan_msgs.founder_cannot_kick_himself;
            break;
        case ClanActionStatus::IS_MEMBER:
            error_msg = clan_msgs.is_member_msg;
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            error_msg = clan_msgs.not_in_clan_msg;
            break;
        case ClanActionStatus::NOT_A_PLAYER:
            error_msg = clan_msgs.prefix + other_player_name + clan_msgs.is_not_member;
            break;
        case ClanActionStatus::PLAYER_DISCONNECTED:
            error_msg = other_player_name + clan_msgs.player_is_disconnected;
            break;
        default:
            throw std::runtime_error("ClanKickCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
