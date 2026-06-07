#include "server_map_data.h"

#include "common/protocol/serializer.h"

ServerMapDataDTO::ServerMapDataDTO(const uint16_t w, const uint16_t h, const GridMatrixDTO& grid,
                                   const std::vector<AllyInfoDTO>& npcs):
        width(w), height(h), grid(grid), npcs(npcs) {}

size_t ServerMapDataDTO::message_size() const {
    return sizeof(width) + sizeof(height) + sizeof(uint8_t) * height * width + sizeof(npcs);
}

void ServerMapDataDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
