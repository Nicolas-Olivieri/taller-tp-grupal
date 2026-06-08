#include "clan.h"

#include <cassert>
#include <format>

void Clan::check_is_in_clan(const std::string& player_name) {
    if (not members.contains(player_name))
        throw std::runtime_error(std::format(
                "El jugador {} llegó a hacerle un pedido al clan {} sin ser miembro ni fundador del mismo",
                player_name, clan_name));
}

ClanActionResult Clan::execute(const ClanActionPayload& payload) {
    const std::string& player_name = payload.player_name;
    const std::string& other_player = payload.other_player;

    switch (payload.type) {
        case ClanActionType::ACCEPT:
            return accept(player_name, other_player);
        case ClanActionType::REJECT:
            return reject(player_name, other_player);
        case ClanActionType::KICK:
            return kick(player_name, other_player);
        case ClanActionType::BAN:
            return ban(player_name, other_player);
        case ClanActionType::LEAVE:
            return leave(player_name);
        case ClanActionType::REVIEW:
            return review(player_name);
        default:
            throw std::runtime_error(
                    std::format("Al clan {}, {} le pidió una acción desconocida", clan_name, player_name));
    }
}

void Clan::recv_join_request(const std::string& player_name) {
    assert(!members.contains(player_name));

    if (!banned_players.contains(player_name))
        joining_requests.insert(player_name);
}

ClanActionResult Clan::accept(const std::string& player_name, const std::string& player_to_accept) {
    check_is_in_clan(player_name);

    if (not is_founder(player_name))
        return ClanActionResult(ClanActionStatus::IS_MEMBER);

    if (members.contains(player_to_accept))
        return ClanActionResult(ClanActionStatus::IS_ALREADY_MEMBER);

    if (banned_players.contains(player_to_accept))
        return ClanActionResult(ClanActionStatus::IS_BANNED_PLAYER);

    if (not joining_requests.contains(player_to_accept))
        return ClanActionResult(ClanActionStatus::IS_NOT_IN_JOIN_LIST);

    joining_requests.extract(player_to_accept);
    members.insert(player_to_accept);

    return ClanActionResult(ClanActionStatus::SUCCESS);
}
