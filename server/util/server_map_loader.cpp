#include "server_map_loader.h"

#include <fstream>
#include <netinet/in.h>

ServerMapLoader::ServerMapLoader() : map_path(DATA_PATH "/map/map.bin") {}

ServerMapDataDTO ServerMapLoader::get_server_data() {
    std::ifstream map;

    try {
        map.open(map_path, std::ios::binary);
    } catch (std::ifstream::failure& e) {
        throw MapFileNotFound();
    }

    // Leo el magic number (2 bytes) y los 4 bytes de offset (inicio y fin de bytes del servidor)
    parse_int<uint16_t>(map);
    parse_int<uint32_t>(map);

    uint16_t width = parse_int<uint16_t>(map);
    uint16_t height = parse_int<uint16_t>(map);

    std::vector<std::vector<bool>> grid_values;
    for (int y = 0; y < height; y++) {
        std::vector<bool> row;
        for (int x = 0; x < width; x++) {
            bool value = parse_int<uint8_t>(map);
            row.push_back(value);
        }
        grid_values.push_back(row);
    }
    GridMatrixDTO grid(grid_values);

    uint16_t npc_amount = parse_int<uint16_t>(map);
    std::vector<AllyInfoDTO> npcs;
    for (int i = 0; i < npc_amount; i++) {
        uint8_t id = parse_int<uint8_t>(map);
        uint16_t x = parse_int<uint16_t>(map);
        uint16_t y = parse_int<uint16_t>(map);

        npcs.push_back(AllyInfoDTO{static_cast<AllyType>(id),x,y});
    }

    return {width,height,grid,npcs};
}



template <typename intType>
intType ServerMapLoader::parse_int(std::ifstream& file) {
    intType data = 0;
    file.read(reinterpret_cast<char*>(&data), sizeof(data));

    if (sizeof(intType) == 2) {
        return ntohs(data);
    }

    return data;
}
