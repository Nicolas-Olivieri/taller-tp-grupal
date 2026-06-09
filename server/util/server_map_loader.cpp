#include "server_map_loader.h"

#include <fstream>
#include <utility>
#include <iostream>
#include <vector>

#include <netinet/in.h>

#define MAGIC_NUMBER 0xFAF4

ServerMapLoader::ServerMapLoader(): map_path(DATA_PATH "/map/map.bin") {
    // La acción inmediata a crearse es leerlo (para cliente o servidor), por lo que no se mantiene mucho
    // tiempo abierto
    map.open(map_path, std::ios::binary);
    if (!map.is_open()) {
        throw MapFileNotFound(map_path);
    }

    // Valido que el archivo sea del formato correcto
    auto magic_number = parse_int<uint16_t>();
    if (magic_number != MAGIC_NUMBER) {
        throw InvalidFile(map_path);
    }
}

ServerMapDataDTO ServerMapLoader::get_server_data() {
    // Leo los 4 bytes de offset (inicio y fin de bytes del servidor)
    parse_int<uint16_t>();
    const auto server_end = parse_int<uint16_t>();

    auto width = parse_int<uint16_t>();
    auto height = parse_int<uint16_t>();

    std::vector<std::vector<bool>> grid_values;
    for (int y = 0; y < height; y++) {
        std::vector<bool> row;
        for (int x = 0; x < width; x++) {
            const bool value = parse_int<uint8_t>();
            row.push_back(value);
        }
        grid_values.push_back(row);
    }

    const auto npc_amount = parse_int<uint16_t>();
    std::vector<AllyInfoDTO> npcs;
    for (int i = 0; i < npc_amount; i++) {
        auto id = parse_int<uint8_t>();
        auto x = parse_int<uint16_t>();
        auto y = parse_int<uint16_t>();

        npcs.emplace_back(static_cast<AllyType>(id), x, y);
    }

    // TODO: perdón ori, pero va a haber que tocar cómo se guarda el mapa y cómo se recupera esta info en el
    // server
    map.seekg(server_end);
    std::vector<AssetInfoDTO> tiles = get_assets();

    GridMatrixDTO grid(grid_values, tiles);

    map.close();

    return {width, height, grid, npcs};
}

ClientMapDataDTO ServerMapLoader::get_client_data() {
    const auto server_start = parse_int<uint16_t>();
    const auto server_end = parse_int<uint16_t>();

    auto width = parse_int<uint16_t>();
    auto height = parse_int<uint16_t>();

    // Salteo la parte que corresponde al servidor
    map.ignore(server_end - server_start);

    std::vector<AssetInfoDTO> tiles = get_assets();
    std::vector<AssetInfoDTO> colliders = get_assets();
    std::vector<AssetInfoDTO> npcs = get_assets();

    map.close();

    return {width, height, tiles, colliders, npcs};
}


std::vector<AssetInfoDTO> ServerMapLoader::get_assets() {
    const auto size = parse_int<uint16_t>();
    std::vector<AssetInfoDTO> assets;
    for (int i = 0; i < size; i++) {
        auto id = parse_int<uint8_t>();
        auto x = parse_int<uint16_t>();
        auto y = parse_int<uint16_t>();

        assets.emplace_back(id, x, y);
    }

    return assets;
}

template <typename intType>
intType ServerMapLoader::parse_int() {
    intType data = 0;
    map.read(reinterpret_cast<char*>(&data), sizeof(data));

    if (sizeof(intType) == 2) {
        return ntohs(data);
    }

    return data;
}
