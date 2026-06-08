#ifndef CLAN_ACTION_RESULT_H
#define CLAN_ACTION_RESULT_H

enum class FoundClanResult { SUCCESS, NOT_ENOUGH_LEVEL, ALREADY_IN_CLAN, CLAN_ALREADY_EXISTS, NO_RESULT };

enum class JoinClanResult { SUCCESS, ALREADY_IN_CLAN, NO_RESULT };

enum class FounderClanActionResult { SUCCESS, NOT_FOUNDER, NOT_IN_CLAN, NO_RESULT };

struct ClanActionResult {
    bool clan_exists = true;
    JoinClanResult join_result;
    FounderClanActionResult founder_result;

    ClanActionResult();
};


#endif  // CLAN_ACTION_RESULT_H
