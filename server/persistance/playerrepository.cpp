#include "playerrepository.h"

#include <utility>

#define REQUESTS_LIM UINT8_MAX

PlayerRepository::PlayerRepository(const std::string& database_path, const std::string& index_path):
        database(database_path), index(index_path), async_requests(REQUESTS_LIM) {
    start();
}

void PlayerRepository::run() {
    try {
        while (should_keep_running()) {
            std::unordered_map<std::string, PlayerData> players_data = this->async_requests.pop();
            update_current_players_state(players_data);
        }
    } catch (const ClosedQueue& error) {}
}

void PlayerRepository::update_current_players_state(
        const std::unordered_map<std::string, PlayerData>& players_data) {
    for (auto& [username, data]: players_data) {
        uint32_t offset = index.get(username);
        database.update(data, offset);
    }
}

void PlayerRepository::save_progress(const std::unordered_map<std::string, Player>& players) {
    std::unordered_map<std::string, PlayerData> players_data;

    for (const auto& [username, player]: players) {
        PlayerData data(player);
        players_data[username] = data;
    }

    this->async_requests.push(std::move(players_data));
}

void PlayerRepository::create(const std::string& username, uint8_t archetype, uint8_t race, uint8_t body,
                              uint8_t head) {
    PlayerData data(archetype, race, body, head);

    index.hold_username(username);
    uint32_t offset = database.add(data);
    index.add(username, offset);
}

PlayerData PlayerRepository::get(const std::string& username) {
    uint32_t offset = this->index.get(username);
    return this->database.get(offset);
}

bool PlayerRepository::exists(const std::string& username) { return index.exists(username); }

PlayerRepository::~PlayerRepository() {
    stop();
    this->async_requests.close();
    join();
}
