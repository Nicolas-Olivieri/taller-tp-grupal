#ifndef INFINITE_RECOVERABLES_COMMAND_H
#define INFINITE_RECOVERABLES_COMMAND_H

#include <string>

#include "server/command/command.h"

class InfiniteRecoberableStatsCommand: public Command {
private:
    std::string player_name;

public:
    explicit InfiniteRecoberableStatsCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // INFINITE_RECOVERABLES_COMMAND_H
