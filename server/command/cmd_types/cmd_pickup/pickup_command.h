#ifndef PICKUP_COMMAND_H
#define PICKUP_COMMAND_H

#include <string>

#include "server/command/command.h"

class PickUpCommand: public Command {
private:
    std::string player_name;

    PickUpResult result;

    ChatMessageDTO create_message();

public:
    explicit PickUpCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};

#endif  // PICKUP_COMMAND_H
