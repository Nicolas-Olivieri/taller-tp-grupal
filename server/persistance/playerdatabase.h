#ifndef PLAYERDATABASE_H
#define PLAYERDATABASE_H

#include <string>

#include "playerdata.h"
#include "playerindex.h"

class PlayerDataBase {
private:
    std::mutex mutex;
    std::fstream file;

    void open_file_correctly(const std::string& database_path);

    void set_data_for_network(PlayerData& data);

    void set_data_for_host(PlayerData& data);

    void read_player_data(PlayerData& data, uint32_t offset);

public:
    explicit PlayerDataBase(const std::string& database_path);

    uint32_t add(PlayerData data);

    void update(PlayerData data, uint32_t offset);

    PlayerData get(uint32_t offset);
};

#endif  // PLAYERDATABASE_H
