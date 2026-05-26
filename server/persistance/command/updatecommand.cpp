#include "updatecommand.h"

#include <utility>

UpdatePlayerCommand::UpdatePlayerCommand(std::unordered_map<std::string, PlayerData>&& player_data):
        player_data(std::move(player_data)) {}

void UpdatePlayerCommand::execute(PlayerDataBase& database, PlayerIndex& index) {
    for (auto& data: player_data) {
        uint32_t offset = index.get(data.first);
        database.update(data.second, offset);
    }
}
