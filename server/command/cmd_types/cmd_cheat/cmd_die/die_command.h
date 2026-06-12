#ifndef DIE_COMMAND_H
#define DIE_COMMAND_H

#include <string>

#include "server/command/command.h"

class DieCommand: public Command {
private:
    std::string player_name;

public:
    explicit DieCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};

#endif  // DIE_COMMAND_H
