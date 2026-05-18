#ifndef MOVE_COMMAND_H
#define MOVE_COMMAND_H

#include <string>

#include "command.h"


class MoveCommand: public Command {
private:
    std::string player_name;
    Direction direction;

public:
    MoveCommand(const std::string& player_name, Direction direction);

    void execute(GameWorld& world) override;
};


#endif  // MOVE_COMMAND_H
