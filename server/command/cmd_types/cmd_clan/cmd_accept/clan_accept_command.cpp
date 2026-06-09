#include "clan_accept_command.h"

#include <cassert>
#include <format>

ClanAcceptCommand::ClanAcceptCommand(const std::string& player_name, const std::string& other_player_name):
        player_name(player_name), other_player_name(other_player_name) {}

void ClanAcceptCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(
            ClanActionPayload(ClanActionType::ACCEPT, player_name, other_player_name));
}

void ClanAcceptCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    switch (result.status) {
        case ClanActionStatus::SUCCESS:
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
        case ClanActionStatus::IS_BANNED_PLAYER:
            error_msg = std::format("El jugador {} esta baneado del clan", other_player_name);
            break;
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
            error_msg = std::format(IS_NOT_IN_JOIN_LIST_MSG, other_player_name);
            break;
        case ClanActionStatus::CLAN_IS_FULL:
            error_msg = CLAN_IS_FULL_MSG;
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
