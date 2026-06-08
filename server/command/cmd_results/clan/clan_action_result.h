#ifndef CLAN_ACTION_RESULT_H
#define CLAN_ACTION_RESULT_H

#include <stdexcept>
#include <string>
#include <vector>

enum class FoundClanResult { SUCCESS, NOT_ENOUGH_LEVEL, ALREADY_IN_CLAN, CLAN_ALREADY_EXISTS, NO_RESULT };

enum class JoinClanResult { SUCCESS, ALREADY_IN_CLAN, CLAN_NOT_FOUND, NO_RESULT };

enum class ClanActionStatus {
    // generales
    SUCCESS,
    IS_FOUNDER,
    IS_MEMBER,
    NOT_IN_CLAN,
    NOT_A_PLAYER,
    NO_RESULT,
    // accept()
    IS_ALREADY_MEMBER,
    IS_BANNED_PLAYER,
    IS_NOT_IN_JOIN_LIST


};

struct ClanActionResult {
    bool clan_exists = true;
    ClanActionStatus status;
    std::vector<std::string> clan_members;
    std::vector<std::string> joining_requests;
    std::vector<std::string> banned_players;

    ClanActionResult();

    explicit ClanActionResult(const ClanActionStatus& status);

    ClanActionResult(const ClanActionStatus& status, const std::vector<std::string>& clan_members,
                     std::vector<std::string>& joining_requests,
                     const std::vector<std::string>& banned_players);
};


#endif  // CLAN_ACTION_RESULT_H
