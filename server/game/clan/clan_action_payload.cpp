#include "clan_action_payload.h"

ClanActionPayload::ClanActionPayload(const ClanActionType& type, const std::string& player_name):
        type(type), player_name(player_name), other_player() {}

ClanActionPayload::ClanActionPayload(const ClanActionType& type, const std::string& player_name,
                                     const std::string& other_player):
        type(type), player_name(player_name), other_player(other_player) {}
