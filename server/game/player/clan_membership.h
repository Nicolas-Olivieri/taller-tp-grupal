#ifndef CLAN_MEMBERSHIP_H
#define CLAN_MEMBERSHIP_H


#include <cstdint>
#include <string>

#include "server/persistance/playerdata.h"
class ClanMembership {
    std::string clan_name;
    bool _is_founder;

public:
    ClanMembership();

    ClanMembership(const char* clan_ptr, bool is_founder);

    void join(const std::string& _clan_name);

    void found(const std::string& _clan_name);

    void leave();

    const std::string& get_clan_name() const;

    bool is_founder() const;
};


#endif  // CLAN_MEMBERSHIP_H
