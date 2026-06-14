#ifndef BUY_COMMAND_H
#define BUY_COMMAND_H

#include <string>

#include "server/command/command.h"


class BuyCommand: public Command {
private:
    std::string player_name;
    uint8_t item_id;
    BuyResult result;

public:
    BuyCommand(const std::string& player_name, uint8_t item_id);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // BUY_COMMAND_H
