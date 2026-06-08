#ifndef USE_ITEM_COMMAND_H
#define USE_ITEM_COMMAND_H

#include <string>

#include "server/command/command.h"


class UseItemCommand: public Command {
private:
    std::string player_name;
    uint8_t item_id;

    UseItemResult result;

public:
    UseItemCommand(const std::string& player_name, uint8_t item_id);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // USE_ITEM_COMMAND_H
