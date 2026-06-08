#ifndef UNEQUIP_ITEM_COMMAND_H
#define UNEQUIP_ITEM_COMMAND_H

#include <string>

#include "server/command/command.h"


class UnequipItemCommand: public Command {
private:
    std::string player_name;
    uint8_t item_id;

    UnequipItemResult result;

public:
    UnequipItemCommand(const std::string& player_name, uint8_t item_id);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // UNEQUIP_ITEM_COMMAND_H
