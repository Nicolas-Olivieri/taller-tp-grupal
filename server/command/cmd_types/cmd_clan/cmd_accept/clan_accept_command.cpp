#include "clan_accept_command.h"

#include <cassert>
#include <format>

#include "common/dto/snapshot/actions/action_types/act_clan_accept/clan_accept.h"

ClanAcceptCommand::ClanAcceptCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanAcceptCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(
            ClanActionPayload(ClanActionType::ACCEPT, player_name, other_player_name));
}

void ClanAcceptCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    const ClanMessagesData& clan_msgs = GameConfig::get().get_clan_messages();

    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            builder.add_action(ActionDTO(ClanAcceptDTO(player_name, other_player_name)));
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::CLAN, player_name,
                                             std::format("{} fue aceptado al clan", other_player_name))));
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::CLAN, other_player_name,
                                             std::format("Bienvenido al clan {}!!", other_player_name))));
            // TODO notificar al resto de los miembros? -> Necesito que el success me de el vector de rtas
            // tmb, ahora está vacío
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
        case ClanActionStatus::IS_BANNED_PLAYER:
            error_msg = std::format("El jugador {} esta baneado del clan", other_player_name);
            break;
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
            error_msg = clan_msgs.prefix + other_player_name + clan_msgs.is_not_in_join_list_msg;
            break;
        case ClanActionStatus::CLAN_IS_FULL:
            error_msg = clan_msgs.clan_is_full_msg;
            break;
        case ClanActionStatus::PLAYER_HAS_CLAN:
            error_msg = std::format("{} ya se unio a otro clan", other_player_name);
            break;
        case ClanActionStatus::IS_FOUNDER:
        case ClanActionStatus::NO_RESULT:
        default:
            throw std::runtime_error("ClanAcceptCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
