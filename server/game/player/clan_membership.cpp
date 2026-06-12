#include "clan_membership.h"

#include <cstring>

ClanMembership::ClanMembership(): clan_name(""), _is_founder(false) {}

ClanMembership::ClanMembership(const char* clan_ptr, bool is_founder):
        clan_name(clan_ptr, strnlen(clan_ptr, CLAN_NAME)), _is_founder(is_founder) {}

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
}

const std::string& ClanMembership::get_clan_name() const { return clan_name; }

bool ClanMembership::is_founder() const { return _is_founder; }
