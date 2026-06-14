#ifndef CLAN_H
#define CLAN_H

#include <cstdint>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "server/command/cmd_results/clan/clan_action_result.h"
#include "server/game/player/player.h"
#include "server/persistance/clan_data.h"

#include "clan_action_payload.h"

class Clan {
    std::string clan_name;
    std::string founder;
    std::unordered_set<std::string> members;

    std::unordered_set<std::string> joining_requests;
    std::unordered_set<std::string> banned_players;


public:
    explicit Clan(const ClanData& data);

    /// Principales operaciones
    // Fundar clan
    Clan(const std::string& clan_name, const std::string& founder_name);

    ClanActionResult execute(const ClanActionPayload& payload);

    void recv_join_request(const std::string& player_name);

    void remove(const std::string& player_name);

    void set_buffed_players(std::unordered_map<std::string, Player>& world_players);

private:
    ClanActionResult accept(const std::string& player_name, const std::string& player_to_accept);

    ClanActionResult reject(const std::string& player_name, const std::string& player_to_reject);

    ClanActionResult kick(const std::string& player_name, const std::string& player_to_kick);

    ClanActionResult ban(const std::string& player_name, const std::string& player_to_ban);

    ClanActionResult leave(const std::string& player_name);

    ClanActionResult review(const std::string& player_name);

    /// Auxiliares
    bool is_founder(const std::string& player_name) const;

    bool is_member(const std::string& player_name) const;

    bool has_pending_request(const std::string& player_name) const;

    bool is_banned(const std::string& player_name) const;

    void check_is_in_clan(const std::string& player_name);
};


#endif  // CLAN_H
