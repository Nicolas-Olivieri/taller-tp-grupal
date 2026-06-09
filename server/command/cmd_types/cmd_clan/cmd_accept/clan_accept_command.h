#ifndef CLAN_ACCEPT_COMMAND_H
#define CLAN_ACCEPT_COMMAND_H


#include <string>

#include "server/command/cmd_results/clan/clan_action_result.h"
#include "server/command/command.h"

class ClanAcceptCommand: public Command {
    std::string player_name;
    std::string other_player_name;
    ClanActionResult result;

public:
    ClanAcceptCommand(const std::string& player_name, const std::string& other_player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};

#endif  // CLAN_ACCEPT_COMMAND_H
