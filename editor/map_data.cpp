#include "map_data.h"
#include <QSet>

MapData::MapData() : tile_id(0) {}

// AÑADIR ASSET::::::::::::::::

int MapData::add_asset(const QPoint position, const AssetData &asset_data) {
    if (asset_data.type == ImageType::TILE) {
        return add_tile(position, asset_data);
    }
    if (asset_data.type == ImageType::COLLIDER) {
        return add_collider(position, asset_data);
    }
    return -1;
}

int MapData::add_tile(const QPoint position, const AssetData &tile_data) {
    // Chequea colisiones para cada una de las celdas que ocupa
    for (int i = 0; i < tile_data.tile_width; i++) {
        for (int j = 0; j < tile_data.tile_height; j++) {
            if (occupied_tiles.contains(QPoint(position.x()+i, position.y()+j))) {
                return -1;
            }
        }
    }

    // Crea la nueva tile del modelo
    const Placement new_tile = {tile_id, position, tile_data};

    // Agrega la tile logica al hash de tiles e indica el id que corresponda en cada celda que ocupe la tile
    placements.insert(tile_id, new_tile);
    for (int i = 0; i < tile_data.tile_width; i++) {
        for (int j = 0; j < tile_data.tile_height; j++) {
            QPoint curr_pos(position.x()+i, position.y()+j);

            // Se puede agregar directamente, porque no debería existir otro valor en el set
            occupied_tiles.insert(curr_pos, QVector{tile_id});

            const QRect& unwalkable_area = tile_data.unwalkable_tiles;
            if (!unwalkable_area.size().isNull() &&
                unwalkable_area.x() <= i && i <= unwalkable_area.x()+unwalkable_area.width() &&
                unwalkable_area.y() <= j && j <= unwalkable_area.y()+unwalkable_area.height()) {
                unwalkable_tiles[curr_pos].append(tile_id);
            }
        }
    }

    tile_id++;
    return new_tile.id;
}

int MapData::add_collider(const QPoint position, const AssetData &collider_data) {
    // Chequea colisiones para cada una de las celdas que ocupa
    for (int i = 0; i < collider_data.tile_width; i++) {
        for (int j = 0; j < collider_data.tile_height; j++) {
            const QPoint curr_pos(position.x()+i, position.y()+j);
            if (!occupied_tiles.contains(curr_pos) || /* No hay una tile colocada */
                (occupied_tiles.contains(curr_pos) && occupied_tiles[curr_pos].length() == 2)) /* Hay tile, pero ya otro collider*/
            {
                return -1;
            }
        }
    }
    const Placement new_tile = {tile_id, position, collider_data};

    placements.insert(tile_id, new_tile);
    for (int i = 0; i < collider_data.tile_width; i++) {
        for (int j = 0; j < collider_data.tile_height; j++) {
            QPoint curr_pos(position.x()+i, position.y()+j);

            // Se agrega directamente a la posición 1 del vector (si o si hay un elemento)
            occupied_tiles[curr_pos].append(tile_id);

            // Si corresponde, agrego las celdas no caminables
            const QRect& unwalkable_area = collider_data.unwalkable_tiles;
            if (unwalkable_area.x() <= i && i <= unwalkable_area.x()+unwalkable_area.width() &&
                unwalkable_area.y() <= j && j <= unwalkable_area.y()+unwalkable_area.height()) {
                unwalkable_tiles[curr_pos].append(tile_id);
            }
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

    // En caso de ser TILE, verifica que no tenga un collider en alguna de sus celdas
    if (asset.type == ImageType::TILE) {
        for (int i = 0; i < asset.tile_width; i++) {
            for (int j = 0; j < asset.tile_height; j++) {
                QPoint curr_pos(position.x()+i, position.y()+j);
                if (occupied_tiles[curr_pos].length() == 2) {
                    return false;
                }
            }
        }
    }

    placements.remove(asset_id);
    for (int i = 0; i < asset.tile_width; i++) {
        for (int j = 0; j < asset.tile_height; j++) {
            QPoint curr_pos(position.x()+i, position.y()+j);

            occupied_tiles.remove(curr_pos);

            if (unwalkable_tiles.contains(curr_pos) && unwalkable_tiles[curr_pos].contains(asset_id)) {
                unwalkable_tiles[curr_pos].removeLast();

                // Si ya no tiene elementos que bloquean la celda, se elimina la entrada
                if (unwalkable_tiles[curr_pos].empty()) {
                    unwalkable_tiles.remove(curr_pos);
                }
            }
        }
    }

    return true;
}

void MapData::clear_all() {
    occupied_tiles.clear();
    placements.clear();
    unwalkable_tiles.clear();
}