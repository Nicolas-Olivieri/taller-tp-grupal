#include "createcommand.h"

#include <utility>

CreatePlayerCommand::CreatePlayerCommand(const std::string& username, PlayerData data):
        username(username), data(std::move(data)) {}

void CreatePlayerCommand::execute(PlayerDataBase& database, PlayerIndex& index) {
    if (index.exists(username))
        throw PlayerAlreadyExists();
    uint32_t offset = database.add(data);
    index.add(username, offset);
}
