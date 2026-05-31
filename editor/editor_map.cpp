#include "editor_map.h"


EditorMap::EditorMap() : tile_id(0) {}

bool EditorMap::add_tile(const QPoint position, const AssetData &tile_data) {
    if (occupied_tiles.contains(position)) {
        return false;
    }
    const Placement new_tile = {tile_id,
                          QRect(position,QSize(tile_data.tile_width, tile_data.tile_height)),
                       tile_data};

    placements.insert(tile_id, new_tile);
    for (int i = 0; i < tile_data.tile_width; i++) {
        for (int j = 0; j < tile_data.tile_height; j++) {
            // Se puede agregar directamente, porque no debería existir otro valor en el set
            occupied_tiles.insert(QPoint(position.x()+i, position.y()+j), QVector{tile_id});
        }
    }

    tile_id++;
    return true;
}

bool EditorMap::add_collider(const QPoint position, const AssetData &collider_data) {
    // no hay un tile de base || (hay tile base && hay un collider)
    if (!occupied_tiles.contains(position) || (occupied_tiles.contains(position) && occupied_tiles[position].length() == 2)) {
        return false;
    }
    const Placement new_tile = {tile_id, QRect(position,QSize(collider_data.tile_width, collider_data.tile_height)), collider_data};

    placements.insert(tile_id, new_tile);
    for (int i = 0; i < collider_data.tile_width; i++) {
        for (int j = 0; j < collider_data.tile_height; j++) {
            // Se agrega directamente a la posición 1 del vector (si o si hay un elemento)
            occupied_tiles[QPoint(position.x()+i, position.y()+j)].append(tile_id);
        }
    }

    tile_id++;
    return true;
}
