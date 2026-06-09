#ifndef CLAN_REJECT_COMMAND_H
#define CLAN_REJECT_COMMAND_H

#include <string>

#include "server/command/cmd_results/clan/clan_action_result.h"
#include "server/command/command.h"

class ClanRejectCommand: public Command {
    std::string player_name;
    std::string other_player_name;
    ClanActionResult result;

public:
    ClanRejectCommand(const std::string& player_name, const std::string& other_player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;

    void notify_success(SnapshotBuilder& builder);
};


#endif  // CLAN_REJECT_COMMAND_H
