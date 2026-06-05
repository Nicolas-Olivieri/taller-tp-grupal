#ifndef WITHDRAW_GOLD_COMMAND_H
#define WITHDRAW_GOLD_COMMAND_H

#include <string>

#include "server/command/command.h"


class WithdrawGoldCommand: public Command {
private:
    std::string player_name;
    uint16_t gold_amount;
    WithdrawGoldResult result;

public:
    WithdrawGoldCommand(const std::string& player_name, uint16_t gold_amount);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // WITHDRAW_GOLD_COMMAND_H
