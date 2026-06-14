#ifndef DEPOSIT_ITEM_COMMAND_H
#define DEPOSIT_ITEM_COMMAND_H

#include <string>

#include "server/command/command.h"


class DepositItemCommand: public Command {
private:
    std::string player_name;
    uint8_t item_id;
    DepositItemResult result;

public:
    DepositItemCommand(const std::string& player_name, uint8_t item_id);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // DEPOSIT_ITEM_COMMAND_H
