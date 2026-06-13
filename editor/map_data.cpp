#include "map_data.h"

#include <QSet>
#include <algorithm>

#include "grid_range.h"

MapData::MapData(): tile_id(0) {}

// AÑADIR ASSET::::::::::::::::

int MapData::add_asset(const QPoint position, const AssetData& asset_data) {
    if (asset_data.type == ImageType::TILE) {
        return add_tile(position, asset_data);
    }
    if (asset_data.type == ImageType::COLLIDER || asset_data.type == ImageType::NPC) {
        return add_collider(position, asset_data);
    }
    return -1;
}

int MapData::add_tile(const QPoint position, const AssetData& tile_data) {
    const GridRange grid_range(position, tile_data.tile_width, tile_data.tile_height);

    // Chequea colisiones para cada una de las celdas que ocupa
    if (std::any_of(grid_range.begin(), grid_range.end(),
                    [this](const QPoint& cell) { return occupied_tiles.contains(cell); })) {
        return -1;
    }

    // Crea la nueva tile del modelo
    const Placement new_tile = {tile_id, position, tile_data};

    // Agrega la tile logica al hash de tiles e indica el id que corresponda en cada celda que ocupe la tile
    asset_counter[tile_data.type]++;
    placements.insert(tile_id, new_tile);
    const QRect unwalkable_area = tile_data.unwalkable_area.translated(position);

    for (const auto& cell: grid_range) {
        occupied_tiles.insert(cell, QVector{tile_id});

        if (!tile_data.inverse_unwalkable && unwalkable_area.contains(cell)) {
            unwalkable_tiles[cell].append(tile_id);
        } else if (tile_data.inverse_unwalkable && !unwalkable_area.contains(cell)) {
            unwalkable_tiles[cell].append(tile_id);
        }
    }

    tile_id++;
    return new_tile.id;
}

int MapData::add_collider(const QPoint position, const AssetData& collider_data) {
    const GridRange grid_range(position, collider_data.tile_width, collider_data.tile_height);

    // Chequea que haya una tile por debajo y no colisione con otro elemento
    if (std::any_of(grid_range.begin(), grid_range.end(), [this](const QPoint& cell) {
            return !occupied_tiles.contains(cell) || occupied_tiles[cell].length() == 2;
        })) {
        return -1;
    }

    const Placement new_tile = {tile_id, position, collider_data};

    placements.insert(tile_id, new_tile);
    asset_counter[collider_data.type]++;
    const QRect unwalkable_area = collider_data.unwalkable_area.translated(position);

    for (const auto& cell: grid_range) {
        occupied_tiles[cell].append(tile_id);

        if ((!collider_data.inverse_unwalkable && unwalkable_area.contains(cell)) ||
            (collider_data.inverse_unwalkable && !unwalkable_area.contains(cell))) {
            unwalkable_tiles[cell].append(tile_id);
        }
    }

    tile_id++;
    return new_tile.id;
}


// BORRAR ASSET::::::::::::::::

bool MapData::erase_asset(const int asset_id) {
    const Placement& placement_data = placements[asset_id];
    const AssetData& asset = placement_data.asset;
    const QPoint& position = placement_data.origin;
    const GridRange grid_range(position, asset.tile_width, asset.tile_height);

    // En caso de ser TILE, verifica que no tenga un collider en alguna de sus celdas
    if (asset.type == ImageType::TILE) {
        if (std::any_of(grid_range.begin(), grid_range.end(),
                        [this](const QPoint& cell) { return occupied_tiles[cell].length() == 2; })) {
            return false;
        }
    }

    placements.remove(asset_id);
    asset_counter[asset.type]--;
    for (const auto& cell: grid_range) {
        occupied_tiles[cell].removeLast();
        if (occupied_tiles[cell].empty()) {
            occupied_tiles.remove(cell);
        }

        if (unwalkable_tiles.contains(cell) && unwalkable_tiles[cell].contains(asset_id)) {
            unwalkable_tiles[cell].removeLast();

            if (unwalkable_tiles[cell].empty()) {
                unwalkable_tiles.remove(cell);
            }
        }
    }
    return true;
}

void MapData::clear_all() {
    asset_counter[ImageType::TILE] = 0;
    asset_counter[ImageType::COLLIDER] = 0;
    asset_counter[ImageType::NPC] = 0;

    occupied_tiles.clear();
    placements.clear();
    unwalkable_tiles.clear();
}
