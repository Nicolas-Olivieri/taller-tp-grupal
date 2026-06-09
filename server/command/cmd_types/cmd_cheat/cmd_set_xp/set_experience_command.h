#ifndef SET_EXPERIENCE_COMMAND_H
#define SET_EXPERIENCE_COMMAND_H

#include <string>

#include "server/command/command.h"

class SetExperienceCommand: public Command {
private:
    std::string player_name;
    uint8_t level;

public:
    SetExperienceCommand(const std::string& player_name, uint8_t level);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // SET_EXPERIENCE_COMMAND_H
