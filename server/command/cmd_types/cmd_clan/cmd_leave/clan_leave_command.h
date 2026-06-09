#ifndef CLAN_LEAVE_COMMAND_H
#define CLAN_LEAVE_COMMAND_H

#include <string>

#include "server/command/cmd_results/clan/clan_action_result.h"
#include "server/command/command.h"

class ClanLeaveCommand: public Command {
    std::string player_name;
    ClanActionResult result;

public:
    explicit ClanLeaveCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // CLAN_LEAVE_COMMAND_H
