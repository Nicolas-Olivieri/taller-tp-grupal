#ifndef CLAN_ACTION_PAYLOAD_H
#define CLAN_ACTION_PAYLOAD_H

#include <string>

enum class ClanActionType { ACCEPT, REJECT, KICK, BAN, LEAVE, REVIEW };

struct ClanActionPayload {
    ClanActionType type;
    std::string player_name;
    std::string other_player;

public:
    ClanActionPayload(const ClanActionType& type, const std::string& player_name);

    ClanActionPayload(const ClanActionType& type, const std::string& player_name,
                      const std::string& other_player);
};


#endif  // CLAN_ACTION_PAYLOAD_H
