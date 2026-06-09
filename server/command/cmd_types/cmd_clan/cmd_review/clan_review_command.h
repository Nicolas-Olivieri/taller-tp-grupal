#ifndef CLAN_REVIEW_COMMAND_H
#define CLAN_REVIEW_COMMAND_H

#include <string>

#include "server/command/cmd_results/clan/clan_action_result.h"
#include "server/command/command.h"

class ClanReviewCommand: public Command {
    std::string player_name;
    ClanActionResult result;

public:
    explicit ClanReviewCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
    void build_review(SnapshotBuilder& builder);
};


#endif  // CLAN_REVIEW_COMMAND_H
