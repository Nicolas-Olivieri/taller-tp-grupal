#include "server_map_data.h"

ServerMapDataDTO::ServerMapDataDTO(const uint16_t w, const uint16_t h, const GridMatrixDTO& grid,
                                   const std::vector<AllyInfoDTO>& npcs):
        width(w), height(h), grid(grid), npcs(npcs) {}
