#ifndef CLAN_ACTION_PAYLOAD_H
#define CLAN_ACTION_PAYLOAD_H

#include <string>

enum class ClanActionType {
    JOIN,
    ACCEPT,
    REJECT,
    KICK,
    BAN,
    LEAVE,
    REVIEW
};

struct ClanActionPayload {
    ClanActionType type;
    std::string player_name;
    std::string target;

public:
    ClanActionPayload(const ClanActionType& type, const std::string& player_name);

    ClanActionPayload(const ClanActionType& type, const std::string& player_name, const std::string& target);
};


#endif  // CLAN_ACTION_PAYLOAD_H
