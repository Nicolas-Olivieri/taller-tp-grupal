#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H

#include <string>
#include <unordered_map>

#include "command.h"

class UpdatePlayerCommand: public PlayerPersistanceCommand {
private:
    std::unordered_map<std::string, PlayerData> player_data;

public:
    explicit UpdatePlayerCommand(std::unordered_map<std::string, PlayerData>&& player_data);

    void execute(PlayerDataBase& database, PlayerIndex& index) override;

    ~UpdatePlayerCommand() = default;
};

#endif  // UPDATECOMMAND_H
