#ifndef PLAYERREPOSITORY_H
#define PLAYERREPOSITORY_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/queue.h"
#include "common/thread/thread.h"
#include "server/game/player/player.h"

#include "clan_data.h"
#include "names_monitor.h"
#include "playerdatabase.h"
#include "playerindex.h"

class PlayerRepository: public Thread {
private:
    PlayerDataBase database;
    PlayerIndex index;
    Queue<std::unordered_map<std::string, PlayerData>> async_requests;
    NamesMonitor players_connected;

    void update_current_players_state(const std::unordered_map<std::string, PlayerData>& players_data);

public:
    PlayerRepository(const std::string& database_path, const std::string& index_path);

    void run() override;

    void save_progress(const Player& player);

    void save_progress(const std::unordered_map<std::string, Player>& players);

    void create(const std::string& username, uint8_t archetype, uint8_t race, uint8_t body, uint8_t head);

    PlayerData get(const std::string& username);

    bool exists(const std::string& username);

    bool is_connected(const std::string& username);

    void connect(const std::string& username);

    // elimina al nombre de la lista de usuarios conectados, si no está el jugador, no hace nada
    void desconnect(const std::string& username);

    std::vector<ClanData> get_saved_clans();

    ~PlayerRepository();
};

#endif  // PLAYERREPOSITORY_H
