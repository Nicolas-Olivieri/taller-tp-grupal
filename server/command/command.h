#ifndef COMMAND_H
#define COMMAND_H

#include "common/dto/snapshot/snapshot_builder.h"
#include "server/connection/event_broadcaster.h"
#include "server/game/game_world.h"


class Command {
public:
    virtual void execute(GameWorld& world) = 0;

    virtual void build_snapshot(SnapshotBuilder& builder) = 0;

    virtual ~Command() = default;
};


#endif  // COMMAND_H
