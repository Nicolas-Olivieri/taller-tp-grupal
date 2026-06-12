#ifndef SELL_COMMAND_H
#define SELL_COMMAND_H

#include <string>

#include "server/command/command.h"


class SellCommand: public Command {
private:
    std::string player_name;
    uint8_t item_id;
    SellResult result;

public:
    SellCommand(const std::string& player_name, uint8_t item_id);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // SELL_COMMAND_H
