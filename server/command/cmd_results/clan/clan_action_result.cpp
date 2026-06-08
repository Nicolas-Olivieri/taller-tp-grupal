#include "clan_action_result.h"

ClanActionResult::ClanActionResult(): clan_exists(false), status(ClanActionStatus::NO_RESULT) {}

ClanActionResult::ClanActionResult(const ClanActionStatus& status): status(status) {}

ClanActionResult::ClanActionResult(const ClanActionStatus& status,
                                   const std::vector<std::string>& clan_members):
        status(status), clan_members(clan_members) {}
