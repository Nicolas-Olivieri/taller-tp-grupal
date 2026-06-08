#ifndef DROP_ITEM_COMMAND_H
#define DROP_ITEM_COMMAND_H

#include <string>

#include "server/command/command.h"


class DropItemCommand: public Command {
private:
    std::string player_name;
    uint8_t item_id;

    DropItemResult result;

public:
    DropItemCommand(const std::string& player_name, uint8_t item_id);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // DROP_ITEM_COMMAND_H
