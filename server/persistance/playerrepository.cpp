#include "playerrepository.h"

#include <utility>

#include "command/createcommand.h"
#include "command/updatecommand.h"

#define REQUESTS_LIM UINT8_MAX

PlayerRepository::PlayerRepository(const std::string& database_path, const std::string& index_path):
        database(database_path), index(index_path), requests(REQUESTS_LIM) {
    start();
}

void PlayerRepository::run() {
    try {
        while (should_keep_running()) {
            std::unique_ptr<PlayerPersistanceCommand> cmd = this->requests.pop();
            cmd->execute(this->database, this->index);
        }
    } catch (const ClosedQueue& error) {}
}

void PlayerRepository::save_progress(const std::string& player_name, const Player& player) {
    PlayerData data(player);

    std::unordered_map<std::string, PlayerData> players_data = {{player_name, data}};
    this->requests.push(std::make_unique<UpdatePlayerCommand>(std::move(players_data)));
}

void PlayerRepository::save_progress(const std::unordered_map<std::string, Player>& players) {
    std::unordered_map<std::string, PlayerData> players_data;

    for (const auto& player: players) {
        PlayerData data(player.second);
        players_data[player.first] = data;
    }

    this->requests.push(std::make_unique<UpdatePlayerCommand>(std::move(players_data)));
}

void PlayerRepository::create(const std::string& username, uint8_t archetype, uint8_t race, uint8_t body,
                              uint8_t head) {
    PlayerData data(archetype, race, body, head);
    this->requests.push(std::make_unique<CreatePlayerCommand>(username, data));
}

PlayerData PlayerRepository::get(const std::string& username) {
    uint32_t offset = this->index.get(username);
    return this->database.get(offset);
}

PlayerRepository::~PlayerRepository() {
    stop();
    this->requests.close();
    join();
}
