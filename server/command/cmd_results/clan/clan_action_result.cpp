#include "clan_action_result.h"

#include <utility>

ClanActionResult::ClanActionResult(): clan_exists(false), status(ClanActionStatus::NO_RESULT) {}

ClanActionResult::ClanActionResult(const ClanActionStatus& status): status(status) {}

ClanActionResult::ClanActionResult(const ClanActionStatus& status, std::vector<std::string>&& clan_members,
                                   std::vector<std::string>&& joining_requests,
                                   std::vector<std::string>&& banned_players):
        status(status),
        clan_members(std::move(clan_members)),
        joining_requests(std::move(joining_requests)),
        banned_players(std::move(banned_players)) {}
