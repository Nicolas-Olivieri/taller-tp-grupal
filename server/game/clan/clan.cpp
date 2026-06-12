#include "clan.h"

#include <cassert>
#include <format>
#include <ranges>
#include <utility>

#include "server/config/game_config.h"
#include "server/game/position.h"

bool Clan::is_founder(const std::string& player_name) const { return player_name == founder; }

bool Clan::is_member(const std::string& player_name) const { return members.contains(player_name); }

bool Clan::has_pending_request(const std::string& player_name) const {
    return joining_requests.contains(player_name);
}

bool Clan::is_banned(const std::string& player_name) const { return banned_players.contains(player_name); }

void Clan::check_is_in_clan(const std::string& player_name) {
    if (not(is_founder(player_name) or is_member(player_name)))
        throw std::runtime_error(std::format(
                "El jugador {} llegó a hacerle un pedido al clan {} sin ser miembro ni fundador del mismo",
                player_name, clan_name));
}

Clan::Clan(const std::string& clan_name, const std::string& founder_name):
        clan_name(clan_name), founder(founder_name) {}

Clan::Clan(const ClanData& data):
        clan_name(data.clan_name),
        founder(data.founder_name),
        members(data.members.begin(), data.members.end()) {}

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
            assert(other_player.empty());
            return leave(player_name);
        case ClanActionType::REVIEW:
            assert(other_player.empty());
            return review(player_name);
        default:
            throw std::runtime_error(
                    std::format("Al clan {}, {} le pidió una acción desconocida", clan_name, player_name));
    }
}

void Clan::recv_join_request(const std::string& player_name) {
    assert(not is_member(player_name));

    if (not is_banned(player_name))
        joining_requests.insert(player_name);
}

ClanActionResult Clan::accept(const std::string& player_name, const std::string& player_to_accept) {
    check_is_in_clan(player_name);

    if (not is_founder(player_name))
        return ClanActionResult(ClanActionStatus::IS_MEMBER);

    if (is_member(player_to_accept))
        return ClanActionResult(ClanActionStatus::IS_ALREADY_MEMBER);

    if (not has_pending_request(player_to_accept))
        return ClanActionResult(ClanActionStatus::IS_NOT_IN_JOIN_LIST);

    if (is_banned(player_to_accept))
        return ClanActionResult(ClanActionStatus::IS_BANNED_PLAYER);

    if (GameConfig::get().get_clan_constats().max_members_per_clan == members.size() + 1)
        return ClanActionResult(ClanActionStatus::CLAN_IS_FULL);

    assert(has_pending_request(player_to_accept));
    joining_requests.extract(player_to_accept);
    members.insert(player_to_accept);

    return ClanActionResult(ClanActionStatus::SUCCESS);
}

ClanActionResult Clan::reject(const std::string& player_name, const std::string& player_to_reject) {
    check_is_in_clan(player_name);

    if (not is_founder(player_name))
        return ClanActionResult(ClanActionStatus::IS_MEMBER);

    if (is_member(player_to_reject))
        return ClanActionResult(ClanActionStatus::IS_ALREADY_MEMBER);

    if (not has_pending_request(player_to_reject))
        return ClanActionResult(ClanActionStatus::IS_NOT_IN_JOIN_LIST);

    joining_requests.extract(player_to_reject);

    return ClanActionResult(ClanActionStatus::SUCCESS);
}

ClanActionResult Clan::kick(const std::string& player_name, const std::string& player_to_kick) {
    check_is_in_clan(player_name);

    if (not is_founder(player_name))
        return ClanActionResult(ClanActionStatus::IS_MEMBER);

    // El fundador trata de kickearse a sí mismo
    if (is_founder(player_to_kick))
        return ClanActionResult(ClanActionStatus::IS_FOUNDER);

    if (not is_member(player_to_kick)) {
        return ClanActionResult(ClanActionStatus::NOT_A_PLAYER);
    }

    assert(members.contains(player_to_kick));
    members.extract(player_to_kick);

    return ClanActionResult(ClanActionStatus::SUCCESS);
}

ClanActionResult Clan::ban(const std::string& player_name, const std::string& player_to_ban) {
    check_is_in_clan(player_name);

    if (not is_founder(player_name))
        return ClanActionResult(ClanActionStatus::IS_MEMBER);

    // El fundador trata de banearse a sí mismo
    if (is_founder(player_to_ban))
        return ClanActionResult(ClanActionStatus::IS_FOUNDER);

    if (has_pending_request(player_to_ban))
        joining_requests.extract(player_to_ban);

    if (is_member(player_to_ban)) {
        assert(kick(player_name, player_to_ban).status == ClanActionStatus::SUCCESS);
    }

    banned_players.insert(player_to_ban);
    return ClanActionResult(ClanActionStatus::SUCCESS);
}

ClanActionResult Clan::review(const std::string& player_name) {
    check_is_in_clan(player_name);

    if (not is_founder(player_name))
        return ClanActionResult(ClanActionStatus::IS_MEMBER);

    std::vector<std::string> members_vector(members.begin(), members.end());
    std::vector<std::string> requests_vector(joining_requests.begin(), joining_requests.end());
    std::vector<std::string> banned_vector(banned_players.begin(), banned_players.end());

    return ClanActionResult(ClanActionStatus::SUCCESS, std::move(members_vector), std::move(requests_vector),
                            std::move(banned_vector));
}

ClanActionResult Clan::leave(const std::string& player_name) {
    check_is_in_clan(player_name);

    if (is_founder(player_name))
        return ClanActionResult(ClanActionStatus::IS_FOUNDER);

    assert(members.contains(player_name));
    members.extract(player_name);

    return ClanActionResult(ClanActionStatus::SUCCESS);
}

void Clan::remove(const std::string& player_name) {
    if (joining_requests.contains(player_name))
        joining_requests.extract(player_name);
    if (members.contains(player_name))
        members.extract(player_name);
}

void Clan::set_buffed_players(std::unordered_map<std::string, Player>& world_players) {
    std::unordered_map<std::string, Position> players_to_position;

    players_to_position.reserve(members.size() + 1);
    for (const auto& member: members) {
        if (not world_players.contains(member))
            continue;

        if (not world_players.at(member).is_alive())
            continue;

        players_to_position.insert({member, world_players.at(member).get_position()});
    }

    if (world_players.contains(founder) and world_players.at(founder).is_alive())
        players_to_position.insert({founder, world_players.at(founder).get_position()});


    for (const auto& [name, position]: players_to_position) {
        uint8_t near_clan_mates = 0;

        for (const auto& [other_name, other_position]: players_to_position) {
            if (name == other_name)
                continue;

            const float distance = position.distance_to(other_position);
            if (distance <= GameConfig::get().get_clan_constats().max_distance_to_consider_near_clan_mate)
                ++near_clan_mates;
        }
        Player& player = world_players.at(name);
        player.set_near_clan_mates(near_clan_mates);
    }
}
