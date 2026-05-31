#ifndef PLAYERREPOSITORY_H
#define PLAYERREPOSITORY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "common/queue.h"
#include "common/thread/thread.h"
#include "server/game/player/player.h"

#include "playerdatabase.h"
#include "playerindex.h"

class PlayerRepository: public Thread {
private:
    PlayerDataBase database;
    PlayerIndex index;
    Queue<std::unordered_map<std::string, PlayerData>> async_requests;

    void update_current_players_state(const std::unordered_map<std::string, PlayerData>& players_data);

public:
    PlayerRepository(const std::string& database_path, const std::string& index_path);

    void run() override;

    void save_progress(const std::unordered_map<std::string, Player>& players);

    void create(const std::string& username, uint8_t archetype, uint8_t race, uint8_t body, uint8_t head);

    PlayerData get(const std::string& username);

    bool exists(const std::string& username);

    ~PlayerRepository();
};

#endif  // PLAYERREPOSITORY_H
