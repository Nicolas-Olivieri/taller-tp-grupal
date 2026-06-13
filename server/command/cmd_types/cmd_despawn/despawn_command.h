#ifndef DESPAWN_COMMAND_H
#define DESPAWN_COMMAND_H

#include <string>

#include "common/dto/snapshot/snapshot_builder.h"
#include "server/command/command.h"


class DespawnCommand: public Command {
private:
    std::string player_name;
    std::string clan_name;

public:
    explicit DespawnCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // DESPAWN_COMMAND_H
