#ifndef RESURRECT_COMMAND_H
#define RESURRECT_COMMAND_H

#include <string>

#include "server/command/command.h"


class ResurrectCommand: public Command {
private:
    std::string player_name;

public:
    explicit ResurrectCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // RESURRECT_COMMAND_H
