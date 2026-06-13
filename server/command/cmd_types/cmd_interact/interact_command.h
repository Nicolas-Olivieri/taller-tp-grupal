#ifndef INTERACT_COMMAND_H
#define INTERACT_COMMAND_H

#include <string>

#include "server/command/command.h"


class InteractCommand: public Command {
private:
    std::string player_name;
    Position position;

    InteractResult result;
    std::string attacked_clan_name;

public:
    explicit InteractCommand(const std::string& player_name, int x, int y);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;

private:
    void handle_attack(SnapshotBuilder& builder);

    void handle_hit(SnapshotBuilder& builder);

    void handle_dodge(SnapshotBuilder& builder);

    void handle_bind(SnapshotBuilder& builder);
};


#endif  // INTERACT_COMMAND_H
