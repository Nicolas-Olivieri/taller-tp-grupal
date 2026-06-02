#ifndef INTERACT_COMMAND_H
#define INTERACT_COMMAND_H

#include <string>

#include "server/command/command.h"


class InteractCommand: public Command {
private:
    std::string player_name;
    Position position;

public:
    explicit InteractCommand(const std::string& player_name, int x, int y);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // INTERACT_COMMAND_H
