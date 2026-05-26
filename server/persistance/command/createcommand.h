#ifndef CREATECOMMAND_H
#define CREATECOMMAND_H

#include <string>

#include "command.h"

class CreatePlayerCommand: public PlayerPersistanceCommand {
private:
    const std::string& username;
    const PlayerData& data;

public:
    CreatePlayerCommand(const std::string& username, const PlayerData& data);

    void execute(PlayerDataBase& database, PlayerIndex& index) override;

    ~CreatePlayerCommand() = default;
};

#endif  // CREATECOMMAND_H
