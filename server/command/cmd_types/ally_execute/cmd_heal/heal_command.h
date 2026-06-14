#ifndef HEAL_COMMAND_H
#define HEAL_COMMAND_H

#include <string>

#include "server/command/command.h"


class HealCommand: public Command {
private:
    std::string player_name;
    HealResult result;

public:
    explicit HealCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // HEAL_COMMAND_H
