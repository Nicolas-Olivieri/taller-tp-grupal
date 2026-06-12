#include "clan_membership.h"

#include <cassert>
#include <cstring>

#include "server/config/game_config.h"

ClanMembership::ClanMembership(): clan_name(""), _is_founder(false), near_clan_mates(0) {}

ClanMembership::ClanMembership(const char* clan_ptr, bool is_founder):
        clan_name(clan_ptr, strnlen(clan_ptr, CLAN_NAME)), _is_founder(is_founder), near_clan_mates(0) {}

void ClanMembership::join(const std::string& _clan_name) {
    clan_name = _clan_name;
    _is_founder = false;
}

void ClanMembership::found(const std::string& _clan_name) {
    clan_name = _clan_name;
    _is_founder = true;
}

void ClanMembership::leave() {
    clan_name.clear();
    _is_founder = false;
    near_clan_mates = 0;
}

const std::string& ClanMembership::get_clan_name() const { return clan_name; }

bool ClanMembership::is_founder() const { return _is_founder; }

void ClanMembership::set_near_clan_mates(uint8_t amount) {
    assert(amount <= GameConfig::get().get_clan_constats().max_members_per_clan - 1);
    near_clan_mates = amount;
}

float ClanMembership::get_clan_buff_factor() const {
    const float min_buff_factor = 1;


    if (clan_name.empty())
        return min_buff_factor;

    const float near_mates_percentage =
            static_cast<float>(near_clan_mates) /
            static_cast<float>(GameConfig::get().get_clan_constats().max_members_per_clan - 1);

    return min_buff_factor +
           near_mates_percentage *
                   (GameConfig::get().get_clan_constats().max_attack_buff_factor - min_buff_factor);
}
