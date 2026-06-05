#ifndef SERVER_MAP_DATA_H
#define SERVER_MAP_DATA_H
#include <cstdint>
#include <vector>

#include "grid_matrix.h"
#include "common/dto/lobby/ally_info.h"


struct ServerMapDataDTO : public ProtocolMessageDTO {
    uint16_t width;
    uint16_t height;
    GridMatrixDTO grid;
    std::vector<AllyInfoDTO> npcs;

    ServerMapDataDTO(uint16_t w, uint16_t h, const GridMatrixDTO &grid, const std::vector<AllyInfoDTO> &npcs);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif //SERVER_MAP_DATA_H
