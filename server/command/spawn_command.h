#ifndef SPAWN_COMMAND_H
#define SPAWN_COMMAND_H

#include <optional>
#include <string>

#include "common/dto/snapshot/snapshot_builder.h"

#include "command.h"


class SpawnCommand: public Command {
private:
    std::string player_name;
    std::optional<Position> position;

public:
    explicit SpawnCommand(const std::string& player_name);

    explicit SpawnCommand(const std::string& player_name, const Position& position);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // SPAWN_COMMAND_H
