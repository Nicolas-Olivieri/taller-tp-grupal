#ifndef LIST_ITEMS_COMMAND_H
#define LIST_ITEMS_COMMAND_H

#include <string>

#include "server/command/command.h"

class ListItemsCommand: public Command {
private:
    std::string player_name;
    std::string ally;
    ListItemsResult result;

public:
    explicit ListItemsCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // LIST_ITEMS_COMMAND_H
