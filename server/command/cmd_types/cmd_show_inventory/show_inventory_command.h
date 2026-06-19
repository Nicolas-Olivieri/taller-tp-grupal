#ifndef SHOW_INVENTORY_COMMAND_H
#define SHOW_INVENTORY_COMMAND_H

#include <map>
#include <string>

#include "server/command/command.h"

class ShowInventoryCommand: public Command {
private:
    std::string player_name;
    InventoryInfoDTO inventory;

public:
    explicit ShowInventoryCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // SHOW_INVENTORY_COMMAND_H
