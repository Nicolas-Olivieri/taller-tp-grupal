#ifndef CLAN_JOIN_COMMAND_H
#define CLAN_JOIN_COMMAND_H


#include <string>

#include "server/command/command.h"

class ClanJoinCommand: public Command {
    std::string player_name;
    std::string clan_name;
    JoinClanResult result;

public:
    ClanJoinCommand(const std::string& player_name, const std::string& clan_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // TALLER_TP_CLAN_JOIN_COMMAND_H
