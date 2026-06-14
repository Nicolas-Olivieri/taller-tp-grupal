#ifndef MEDITATE_COMMAND_H
#define MEDITATE_COMMAND_H

#include <string>

#include "server/command/command.h"


class MeditateCommand: public Command {
private:
    std::string player_name;

    MeditateResult result;

public:
    explicit MeditateCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // MEDITATE_COMMAND_H
