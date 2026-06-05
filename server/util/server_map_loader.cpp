#include "server_map_loader.h"

#include <fstream>
#include <netinet/in.h>

ServerMapLoader::ServerMapLoader() : map_path(DATA_PATH "/map/map.bin") {}

ServerMapDataDTO ServerMapLoader::get_server_data() {
    std::ifstream map;

    try {
        map.open(map_path, std::ios::binary);
    } catch (std::ifstream::failure& e) {
        throw MapFileNotFound(map_path);
    }

    // Leo el magic number (2 bytes) y los 4 bytes de offset (inicio y fin de bytes del servidor)
    parse_int<uint16_t>(map);
    parse_int<uint32_t>(map);

    auto width = parse_int<uint16_t>(map);
    auto height = parse_int<uint16_t>(map);

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

    auto npc_amount = parse_int<uint16_t>(map);
    std::vector<AllyInfoDTO> npcs;
    for (int i = 0; i < npc_amount; i++) {
        auto id = parse_int<uint8_t>(map);
        auto x = parse_int<uint16_t>(map);
        auto y = parse_int<uint16_t>(map);

        npcs.emplace_back(static_cast<AllyType>(id),x,y);
    }

    return {width,height,grid,npcs};
}

ClientMapDataDTO ServerMapLoader::get_client_data() {
    std::ifstream map;

    try {
        map.open(map_path, std::ios::binary);
    } catch (std::ifstream::failure& e) {
        throw MapFileNotFound(map_path);
    }

    // Leo el magic number (2 bytes)
    parse_int<uint16_t>(map);
    auto server_start = parse_int<uint16_t>(map);
    auto server_end = parse_int<uint16_t>(map);

    auto width = parse_int<uint16_t>(map);
    auto height = parse_int<uint16_t>(map);

    // Salteo la parte
    map.ignore(server_end-server_start);

    std::vector<AssetInfoDTO> tiles = get_assets(map);
    std::vector<AssetInfoDTO> colliders = get_assets(map);
    std::vector<AssetInfoDTO> npcs = get_assets(map);

    return {width, height, tiles,colliders,npcs};
}


std::vector<AssetInfoDTO> ServerMapLoader::get_assets(std::ifstream& map) {
    const auto size = parse_int<uint16_t>(map);
    std::vector<AssetInfoDTO> assets;
    for (int i = 0; i < size; i++) {
        auto id = parse_int<uint8_t>(map);
        auto x = parse_int<uint16_t>(map);
        auto y = parse_int<uint16_t>(map);

        assets.emplace_back(id, x, y);
    }

    return assets;
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
