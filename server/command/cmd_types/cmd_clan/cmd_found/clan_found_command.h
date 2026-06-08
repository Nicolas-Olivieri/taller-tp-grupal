#ifndef CLAN_FOUND_COMMAND_H
#define CLAN_FOUND_COMMAND_H


#include <string>

#include "server/command/command.h"

class ClanFoundCommand: public Command {
    std::string player_name;
    std::string clan_name;
    FoundClanResult result;

public:
    ClanFoundCommand(const std::string& player_name, const std::string& clan_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // CLAN_FOUND_COMMAND_H
