#ifndef WITHDRAW_ITEM_COMMAND_H
#define WITHDRAW_ITEM_COMMAND_H

#include <string>

#include "server/command/command.h"


class WithdrawItemCommand: public Command {
private:
    std::string player_name;
    uint8_t item_id;
    WithdrawItemResult result;

public:
    WithdrawItemCommand(const std::string& player_name, uint8_t item_id);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // WITHDRAW_ITEM_COMMAND_H
