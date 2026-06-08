#include "clan_action_payload.h"

ClanActionPayload::ClanActionPayload(const ClanActionType& type, const std::string& player_name):
        type(type), player_name(player_name), target() {}

ClanActionPayload::ClanActionPayload(const ClanActionType& type, const std::string& player_name,
                                     const std::string& target):
        type(type), player_name(player_name), target(target) {}
