#include "clan_action_result.h"

ClanActionResult::ClanActionResult(): clan_exists(false), status(ClanActionStatus::NO_RESULT) {}

ClanActionResult::ClanActionResult(const ClanActionStatus& status): status(status) {}

ClanActionResult::ClanActionResult(const ClanActionStatus& status,
                                   const std::vector<std::string>& clan_members,
                                   std::vector<std::string>& joining_requests,
                                   const std::vector<std::string>& banned_players):
        status(status),
        clan_members(clan_members),
        joining_requests(joining_requests),
        banned_players(banned_players) {}
