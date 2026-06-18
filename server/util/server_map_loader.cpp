#include "server_map_loader.h"

#include <fstream>
#include <iostream>
#include <vector>

#include <netinet/in.h>

#include "common/dto/snapshot/map/teleport_info.h"
#include "server/config/game_config.h"
#include "server/game/tile.h"

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
    // Leo los 5 bytes de offset (inicio y fin de bytes del servidor)
    parse_int<uint8_t>();
    parse_int<uint64_t>();

    // Parseo la grilla
    auto width = parse_int<uint16_t>();
    auto height = parse_int<uint16_t>();

    std::vector<std::vector<TileInfoDTO>> grid_values;
    for (int y = 0; y < height; y++) {
        std::vector<TileInfoDTO> row;
        for (int x = 0; x < width; x++) {
            const bool walkability = parse_int<uint8_t>();
            const auto floor = parse_int<uint8_t>();

            row.push_back(TileInfoDTO(walkability, floor));
        }
        grid_values.push_back(row);
    }
    GridMatrixDTO grid(grid_values);

    // Parseo los NPCs
    const auto npc_amount = parse_int<uint16_t>();
    std::vector<AllyInfoDTO> npcs;
    for (int i = 0; i < npc_amount; i++) {
        auto id = parse_int<uint8_t>();
        auto x = parse_int<uint16_t>();
        auto y = parse_int<uint16_t>();

        npcs.emplace_back(static_cast<AllyType>(id), x, y);
    }

    // Parseo los teletransportadores
    const auto teleport_amount = parse_int<uint16_t>();
    std::vector<TeleportInfoDTO> teleports;
    for (int i = 0; i < teleport_amount; i++) {
        auto port_a_x = parse_int<uint16_t>();
        auto port_a_y = parse_int<uint16_t>();
        auto port_b_x = parse_int<uint16_t>();
        auto port_b_y = parse_int<uint16_t>();

        teleports.emplace_back(port_a_x, port_a_y, port_b_x, port_b_y);
    }

    map.close();

    return {width, height, grid, npcs, teleports};
}

ClientMapDataDTO ServerMapLoader::get_client_data() {
    const auto server_start = parse_int<uint8_t>();
    const auto server_end = parse_int<uint64_t>();

    auto width = parse_int<uint16_t>();
    auto height = parse_int<uint16_t>();

    std::vector<AssetInfoDTO> safe_zones = get_safe_zones(width, height);

    // Calculo cuántos bytes se leyeron de la grilla (2 bytes por tile) y salteo los sobrantes
    const size_t bytes_read = width * height * 2;
    map.ignore((server_end - server_start) - bytes_read);

    std::vector<AssetInfoDTO> tiles = get_assets();
    std::vector<AssetInfoDTO> colliders = get_assets();
    std::vector<AssetInfoDTO> npcs = get_assets();
    map.close();

    return {width, height, tiles, safe_zones, colliders, npcs};
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

// TODO: No me termina de convencer tener que para el cliente se tenga que consultar esta información
//  desde los datos del servidor... Tal vez el editor podría guardar los datos de zonas seguras (?)
std::vector<AssetInfoDTO> ServerMapLoader::get_safe_zones(const uint16_t width, const uint16_t height) {
    std::vector<AssetInfoDTO> safe_zones;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            parse_int<uint8_t>();
            const uint8_t floor = parse_int<uint8_t>();

            GameConfig& config = GameConfig::get();
            // TODO: Cambiar el uso de la macro por GameConfig::is_safe_zone_floor
            if (config.has_biome_associated(floor) && config.get_biome_id(floor) == SAFE_ZONE_FLOOR)
                safe_zones.emplace_back(SAFE_ZONE_FLOOR, x, y);
        }
    }

    return safe_zones;
}

template <typename intType>
intType ServerMapLoader::parse_int() {
    intType data = 0;
    map.read(reinterpret_cast<char*>(&data), sizeof(data));

    if (sizeof(intType) == 2) {
        return ntohs(data);
    }
    if (sizeof(intType) == 4) {
        return ntohl(data);
    }
    if (sizeof(intType) == 8) {
        return be64toh(data);
    }

    return data;
}
