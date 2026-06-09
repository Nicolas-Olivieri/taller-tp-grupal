
#include "clan_data.h"

#include <cassert>
#include <cstring>

void ClanData::add_player(const std::string& player_name, const PlayerData& player_data) {
    const char* clan_ptr = reinterpret_cast<const char*>(player_data.clan);
    std::string clan(clan_ptr, strnlen(clan_ptr, CLAN_NAME));

    if (clan_name.empty()) {
        clan_name = clan;
    } else {
        assert(clan_name == clan);
    }

    if (player_data.is_founder) {
        founder_name = player_name;
    } else {
        members.push_back(player_name);
    }
}
