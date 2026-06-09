#ifndef CLAN_KICK_COMMAND_H
#define CLAN_KICK_COMMAND_H

#include <string>

#include "server/command/cmd_results/clan/clan_action_result.h"
#include "server/command/command.h"

class ClanKickCommand: public Command {
    std::string player_name;
    std::string other_player_name;
    ClanActionResult result;

public:
    ClanKickCommand(const std::string& player_name, const std::string& other_player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // CLAN_KICK_COMMAND_H
