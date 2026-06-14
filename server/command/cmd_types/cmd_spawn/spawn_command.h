#ifndef SPAWN_COMMAND_H
#define SPAWN_COMMAND_H

#include <string>

#include "common/dto/snapshot/snapshot_builder.h"
#include "server/command/command.h"


class SpawnCommand: public Command {
private:
    std::string player_name;
    PlayerData data;
    std::string clan_name;

public:
    SpawnCommand(const std::string& player_name, const PlayerData& data);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // SPAWN_COMMAND_H
