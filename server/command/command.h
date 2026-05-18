#ifndef COMMAND_H
#define COMMAND_H

#include "server/game/game_world.h"


class Command {
public:
    virtual void execute(GameWorld& world) = 0;

    virtual ~Command() = default;
};


#endif  // COMMAND_H
