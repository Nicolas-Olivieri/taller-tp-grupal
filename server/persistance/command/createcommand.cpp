#include "createcommand.h"

CreatePlayerCommand::CreatePlayerCommand(const std::string& username, const PlayerData& data):
        username(username), data(data) {}

void CreatePlayerCommand::execute(PlayerDataBase& database, PlayerIndex& index) {
    if (index.exists(username))
        throw PlayerAlreadyExists();
    uint32_t offset = database.add(data);
    index.add(username, offset);
}
