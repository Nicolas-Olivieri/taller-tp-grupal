#include "editor_map.h"


EditorMap::EditorMap() : tile_id(0) {}

int EditorMap::add_asset(const QPoint position, const AssetData &asset_data) {
    if (asset_data.type == ImageType::TILE) {
        return add_tile(position, asset_data);
    }
    if (asset_data.type == ImageType::COLLIDER) {
        return add_collider(position, asset_data);
    }
    return -1;
}


int EditorMap::add_tile(const QPoint position, const AssetData &tile_data) {
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
            // Se puede agregar directamente, porque no debería existir otro valor en el set
            occupied_tiles.insert(QPoint(position.x()+i, position.y()+j), QVector{tile_id});
        }
    }

    tile_id++;
    return new_tile.id;
}

int EditorMap::add_collider(const QPoint position, const AssetData &collider_data) {
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
            // Se agrega directamente a la posición 1 del vector (si o si hay un elemento)
            occupied_tiles[QPoint(position.x()+i, position.y()+j)].append(tile_id);
        }
    }

    tile_id++;
    return new_tile.id;
}
