#ifndef KILL_CREATURES_COMMAND_H
#define KILL_CREATURES_COMMAND_H


#include <string>

#include "server/command/command.h"

class KillCreaturesCommand: public Command {
private:
    std::string player_name;

public:
    explicit KillCreaturesCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // KILL_CREATURES_COMMAND_H
