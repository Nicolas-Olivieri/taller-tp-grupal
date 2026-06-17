#include "server_map_data.h"

#include "common/protocol/serializer.h"

ServerMapDataDTO::ServerMapDataDTO(const uint16_t w, const uint16_t h, const GridMatrixDTO& grid,
                                   const std::vector<AllyInfoDTO>& npcs,
                                   const std::vector<TeleportInfoDTO>& teleports):
        width(w), height(h), grid(grid), npcs(npcs), teleports(teleports) {}

size_t ServerMapDataDTO::message_size() const {
    const size_t npcs_size = npcs.empty() ? 0 : npcs.size() * npcs[0].message_size();
    const size_t teleport_size = teleports.empty() ? 0 : teleports.size() * teleports[0].message_size();
    return sizeof(width) + sizeof(height) + sizeof(uint16_t) * height * width + sizeof(uint16_t) + npcs_size +
           sizeof(uint16_t) + teleport_size;
}

void ServerMapDataDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
