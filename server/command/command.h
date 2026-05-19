#ifndef COMMAND_H
#define COMMAND_H

#include "server/connection/event_broadcaster.h"
#include "server/game/game_world.h"


class Command {
public:
    virtual void execute(GameWorld& world) = 0;

    virtual void broadcast(EventBroadcaster& broadcaster) = 0;

    virtual ~Command() = default;
};


#endif  // COMMAND_H
