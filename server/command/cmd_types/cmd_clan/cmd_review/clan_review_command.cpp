#include "clan_review_command.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <vector>

ClanReviewCommand::ClanReviewCommand(const std::string& player_name): player_name(player_name) {}

void ClanReviewCommand::execute(GameWorld& world) {
    result = world.execute_clan_action(ClanActionPayload(ClanActionType::BAN, player_name));
}

void ClanReviewCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    switch (result.status) {
        case ClanActionStatus::SUCCESS:
            build_review(builder);
            return;

        case ClanActionStatus::IS_MEMBER:
            error_msg = IS_MEMBER_MSG;
            break;
        case ClanActionStatus::NOT_IN_CLAN:
            error_msg = NOT_IN_CLAN_MSG;
            break;
        case ClanActionStatus::IS_FOUNDER:
        case ClanActionStatus::NO_RESULT:
        case ClanActionStatus::NOT_A_PLAYER:
        case ClanActionStatus::IS_ALREADY_MEMBER:
        case ClanActionStatus::IS_BANNED_PLAYER:
        case ClanActionStatus::IS_NOT_IN_JOIN_LIST:
        default:
            throw std::runtime_error("ClanReviewCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
void ClanReviewCommand::build_review(SnapshotBuilder& builder) {
    std::vector<std::string> lines;
    lines.reserve(1 + 2 + 2 + result.banned_players.size() + 1 + result.clan_members.size() + 1 +
                  result.joining_requests.size());

    // TODO borrar los fors comentados si fuciona bien

    lines.push_back("    Revision del clan:");
    lines.push_back("  Fundador:");
    lines.push_back(player_name);
    lines.push_back(std::format("  Miembros: "));
    std::copy(result.clan_members.begin(), result.clan_members.end(), std::back_inserter(lines));
    //    for (const auto& member: result.clan_members) {
    //        lines.push_back(member);
    //    }
    lines.push_back(
            std::format("  Capacidad del clan: {}/{}", result.clan_members.size() + 1, Clan::MAX_MEMBERS));
    std::copy(result.joining_requests.begin(), result.joining_requests.end(), std::back_inserter(lines));
    //    lines.push_back("  Solicitudes de union del clan");
    //    for (const auto& request: result.joining_requests) {
    //        lines.push_back(request);
    //    }
    lines.push_back("  Jugadores baneados del clan");
    std::copy(result.banned_players.begin(), result.banned_players.end(), std::back_inserter(lines));
    //    for (const auto& banned_player: result.banned_players) {
    //        lines.push_back(banned_player);
    //    }

    assert(lines.size() < UINT16_MAX);
    builder.add_action(ActionDTO(ChatListDTO(MessageType::CLAN, lines, player_name)));
}
