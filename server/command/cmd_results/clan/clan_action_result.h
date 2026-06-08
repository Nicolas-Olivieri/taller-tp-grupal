#ifndef CLAN_ACTION_RESULT_H
#define CLAN_ACTION_RESULT_H

enum class CreateClanResult {
    SUCCESS,
    NOT_ENOUGH_LEVEL,
    ALREADY_IN_CLAN,
    CLAN_ALREADY_EXISTS,
    NO_RESULT
};

enum class JoinClanResult {
    SUCCESS,
    ALREADY_IN_CLAN,
    NO_RESULT
};

enum class FounderClanResult {
    SUCCESS,
    NOT_FOUNDER,
    NOT_IN_CLAN,
    NO_RESULT
};

struct ClanActionResult {
    JoinClanResult join_result;
    FounderClanResult founder_result;


};


#endif  // CLAN_ACTION_RESULT_H
