#ifndef TELEPORT_COMMAND_H
#define TELEPORT_COMMAND_H

#include <string>

#include "server/command/command.h"

class TeleportCommand: public Command {
private:
    std::string player_name;
    TeleportResult result;

public:
    explicit TeleportCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // TELEPORT_COMMAND_H
