#include "map_data.h"

#include <QSet>
#include <algorithm>

#include "grid_range.h"

MapData::MapData(): tile_id(0) {}

// AÑADIR ASSET::::::::::::::::

int MapData::add_asset(const QPoint position, const AssetData& asset_data) {

    switch (asset_data.type) {
        case ImageType::TILE:
            return add_tile(position, asset_data);

        case ImageType::COLLIDER:
            return add_collider(position, asset_data);

        case ImageType::NPC:
            const int id = add_collider(position, asset_data);
            if (asset_data.id == TELEPORT_NPC) {
                add_teleport(id);
            }
            return id;
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
    const QRect unwalkable_area = collider_data.unwalkable_area.translated(position);

    // Chequea que toda el area del collider tenga un tile por debajo y que su zona no caminable no colisione
    // con la de otro collider (las zonas caminables puede superponerse)
    if (std::any_of(grid_range.begin(), grid_range.end(), [this, unwalkable_area](const QPoint& cell) {
            return !occupied_tiles.contains(cell) ||
                   (unwalkable_area.contains(cell) && occupied_tiles[cell].length() == 2);
        })) {
        return -1;
    }

    const Placement new_tile = {tile_id, position, collider_data};

    placements.insert(tile_id, new_tile);
    asset_counter[collider_data.type]++;
    for (const auto& cell: grid_range) {
        if ((!collider_data.inverse_unwalkable && unwalkable_area.contains(cell)) ||
            (collider_data.inverse_unwalkable && !unwalkable_area.contains(cell))) {
            occupied_tiles[cell].append(tile_id);
            unwalkable_tiles[cell].append(tile_id);
        }
    }

    tile_id++;
    return new_tile.id;
}

void MapData::add_teleport(const int id) {
    if (curr_teleport_pair.empty()) {
        curr_teleport_pair.append(id);
        return;
    }

    curr_teleport_pair.append(id);
    int point_a = curr_teleport_pair[0];
    int point_b = curr_teleport_pair[1];
    teleport_pairs.insert({{point_a, point_b}, {point_b, point_a}});
    curr_teleport_pair.clear();
}


// BORRAR ASSET::::::::::::::::

bool MapData::erase_asset(const int asset_id) {
    const Placement& placement_data = placements[asset_id];

    switch (placement_data.asset.type) {
        case ImageType::TILE:
            return erase_tile(placement_data);

        case ImageType::COLLIDER:
            return erase_collider(placement_data);

        case ImageType::NPC:
            erase_collider(placement_data);
            if (placement_data.asset.id == TELEPORT_NPC) {
                erase_teleport(placement_data.id);
            }
            return true;
    }

    return false;
}

bool MapData::erase_tile(const Placement& placement) {
    const AssetData& asset = placement.asset;
    const GridRange grid_range(placement.origin, asset.tile_width, asset.tile_height);

    if (std::any_of(grid_range.begin(), grid_range.end(),
                    [this](const QPoint& cell) { return occupied_tiles[cell].length() == 2; })) {
        return false;
    }

    placements.remove(placement.id);
    asset_counter[asset.type]--;

    for (const auto& cell: grid_range) {
        occupied_tiles.remove(cell);

        if (unwalkable_tiles.contains(cell) && unwalkable_tiles[cell].contains(placement.id)) {
            unwalkable_tiles.remove(cell);
        }
    }
    return true;
}

bool MapData::erase_collider(const Placement& placement) {
    const AssetData& asset = placement.asset;
    const QRect unwalkable_area = asset.unwalkable_area.translated(placement.origin);
    const GridRange grid_range(unwalkable_area.topLeft(), unwalkable_area.width(), unwalkable_area.height());

    placements.remove(placement.id);
    asset_counter[asset.type]--;
    for (const auto& cell: grid_range) {
        occupied_tiles[cell].removeLast();
        unwalkable_tiles[cell].removeLast();
    }
    return true;
}

void MapData::erase_teleport(const int point_a) {
    if (curr_teleport_pair.contains(point_a)) {
        curr_teleport_pair.clear();
        return;
    }

    const int point_b = teleport_pairs[point_a];
    teleport_pairs.remove(point_a);
    teleport_pairs.remove(point_b);
}

// ZONA SEGURA::::::::::::::::

QSet<QPair<int, QPoint>> MapData::add_safe_tiles(const QPoint origin_position, const int width,
                                                 const int height) {
    const GridRange grid_range(origin_position, width, height);
    QSet<QPair<int, QPoint>> valid_new_tiles;

    for (const auto& cell: grid_range) {
        if (!occupied_tiles.contains(cell)) {
            continue;  // No hay una tile dibujada en donde se quiere colocar una zona segura
        }

        if (!safe_zone_tiles.contains(cell)) {
            valid_new_tiles.insert(QPair(tile_id, cell));
            tile_id++;
        }
        safe_zone_tiles.insert(cell);
    }
    return valid_new_tiles;
}

void MapData::erase_safe_tile(const QPoint position) { safe_zone_tiles.remove(position); }


void MapData::clear_all() {
    asset_counter[ImageType::TILE] = 0;
    asset_counter[ImageType::COLLIDER] = 0;
    asset_counter[ImageType::NPC] = 0;

    placements.clear();
    occupied_tiles.clear();
    unwalkable_tiles.clear();
    safe_zone_tiles.clear();

    curr_teleport_pair.clear();
    teleport_pairs.clear();
}
