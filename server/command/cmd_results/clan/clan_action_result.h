#ifndef CLAN_ACTION_RESULT_H
#define CLAN_ACTION_RESULT_H

#include <stdexcept>
#include <string>
#include <vector>

#define NOT_IN_CLAN_MSG "Debes estar en un clan para realizar esta accion"
#define NOT_A_PLAYER_MSG "El jugador {} no existe"
#define IS_MEMBER_MSG "Solo el fundador del clan puede realizar esa accion"
#define IS_ALREADY_MEMBER_MSG "El jugador {} ya es un miembro del clan"
#define IS_NOT_IN_JOIN_LIST_MSG "El jugador {} no solicito unirse al clan"
#define CLAN_IS_FULL_MSG "El clan ya esta lleno"

enum class FoundClanResult {
    SUCCESS,
    NOT_ENOUGH_LEVEL,
    ALREADY_IN_CLAN,
    CLAN_ALREADY_EXISTS,
    CLAN_NAME_LONG,
    NO_RESULT
};

enum class JoinClanResult { SUCCESS, ALREADY_IN_CLAN, CLAN_NOT_FOUND, NO_RESULT };

enum class ClanActionStatus {
    // generales
    SUCCESS,
    IS_FOUNDER,
    IS_MEMBER,
    NOT_IN_CLAN,
    NOT_A_PLAYER,
    NO_RESULT,
    // accept(), reject(), kick()
    IS_ALREADY_MEMBER,
    IS_BANNED_PLAYER,
    IS_NOT_IN_JOIN_LIST,
    CLAN_IS_FULL,
    PLAYER_HAS_CLAN,

};

struct ClanActionResult {
    bool clan_exists = true;
    ClanActionStatus status;
    std::vector<std::string> clan_members;
    std::vector<std::string> joining_requests;
    std::vector<std::string> banned_players;

    ClanActionResult();

    explicit ClanActionResult(const ClanActionStatus& status);

    ClanActionResult(const ClanActionStatus& status, std::vector<std::string>&& clan_members,
                     std::vector<std::string>&& joining_requests, std::vector<std::string>&& banned_players);
};


#endif  // CLAN_ACTION_RESULT_H
