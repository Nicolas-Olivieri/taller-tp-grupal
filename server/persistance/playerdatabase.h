#ifndef PLAYERDATABASE_H
#define PLAYERDATABASE_H

#include <string>

#include "playerdata.h"
#include "playerindex.h"

class PlayerDataBase {
private:
    std::mutex mutex;
    std::fstream file;

public:
    explicit PlayerDataBase(const std::string& database_path);

    uint32_t add(PlayerData data);

    void update(PlayerData data, uint32_t offset);

    PlayerData get(uint32_t offset);
};

#endif  // PLAYERDATABASE_H
