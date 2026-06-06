#ifndef WORLD_UPDATE_STATUS_H
#define WORLD_UPDATE_STATUS_H

#include <string>
#include <vector>

#include "creatures/updatestatus.h"


// TODO: Encontrarle un mejor lugar a nivel directorios
struct WorldUpdateStatus {
    std::vector<CreatureUpdateStatus> creatures;
    std::vector<std::string> resurrected_players;
};


#endif  // WORLD_UPDATE_STATUS_H
