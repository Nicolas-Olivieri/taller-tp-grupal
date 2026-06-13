#ifndef GRID_MATRIX_H
#define GRID_MATRIX_H
#include <vector>

#include "tile_info.h"
#include "common/dto/message.h"

struct GridMatrixDTO: public ProtocolMessageDTO {
    std::vector<std::vector<TileInfoDTO>> tiles_info;

    explicit GridMatrixDTO(const std::vector<std::vector<TileInfoDTO>>& walkable_tiles);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // GRID_MATRIX_H
