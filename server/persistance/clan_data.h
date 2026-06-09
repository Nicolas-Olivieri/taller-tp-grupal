#ifndef CLAN_DATA_H
#define CLAN_DATA_H


#include <string>
#include <vector>

#include "playerdata.h"

struct ClanData {
    std::string clan_name;
    std::string founder_name;
    std::vector<std::string> members;

    void add_player(const std::string& player_name, const PlayerData& player_data);
};


#endif  // CLAN_DATA_H
