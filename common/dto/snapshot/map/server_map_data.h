#ifndef SERVER_MAP_DATA_H
#define SERVER_MAP_DATA_H
#include <cstdint>
#include <vector>

#include "common/dto/lobby/ally_info.h"

#include "grid_matrix.h"
#include "teleport_info.h"

struct ServerMapDataDTO {
    uint16_t width;
    uint16_t height;
    GridMatrixDTO grid;
    std::vector<AllyInfoDTO> npcs;
    std::vector<TeleportInfoDTO> teleports;

    ServerMapDataDTO(uint16_t w, uint16_t h, const GridMatrixDTO& grid, const std::vector<AllyInfoDTO>& npcs,
                     const std::vector<TeleportInfoDTO>& teleports);
};


#endif  // SERVER_MAP_DATA_H
