#ifndef GAIN_GOLD_COMMAND_H
#define GAIN_GOLD_COMMAND_H

#include <string>

#include "server/command/command.h"

class GainGoldCommand: public Command {
private:
    std::string player_name;
    uint16_t gold_amount;

public:
    GainGoldCommand(const std::string& player_name, uint16_t gold_amount);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};

#endif  // GAIN_GOLD_COMMAND_H
