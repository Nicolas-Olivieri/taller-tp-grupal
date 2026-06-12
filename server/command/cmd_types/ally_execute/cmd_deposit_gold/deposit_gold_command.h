#ifndef DEPOSIT_GOLD_COMMAND_H
#define DEPOSIT_GOLD_COMMAND_H

#include <string>

#include "server/command/command.h"


class DepositGoldCommand: public Command {
private:
    std::string player_name;
    uint16_t gold_amount;
    DepositGoldResult result;

public:
    DepositGoldCommand(const std::string& player_name, uint16_t gold_amount);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // DEPOSIT_GOLD_COMMAND_H
