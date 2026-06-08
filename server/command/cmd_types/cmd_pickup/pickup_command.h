#ifndef PICKUP_COMMAND_H
#define PICKUP_COMMAND_H

#include <string>

#include "server/command/cmd_results/pickup/pickup_result.h"
#include "server/command/command.h"

class PickUpCommand: public Command {
private:
    std::string player_name;
    Position position;

    PickUpResult result;

public:
    PickUpCommand(const std::string& player_name, uint16_t x, uint16_t y);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};

#endif  // PICKUP_COMMAND_H
