#ifndef GET_ITEM_COMMAND_H
#define GET_ITEM_COMMAND_H

#include <string>

#include "server/command/command.h"

class GetItemCommand: public Command {
private:
    std::string player_name;
    uint8_t item;

public:
    GetItemCommand(const std::string& player_name, uint8_t item);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // GET_ITEM_COMMAND_H
