#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <qhash.h>
#include <qpoint.h>

#include "components.h"


class MapData {
    friend class MapSaver;
    friend class MapLoader;

private:
    int tile_id;
    QHash<int, Placement> placements;
    QHash<QPoint, QVector<int>> occupied_tiles;
    QHash<QPoint, QVector<int>> unwalkable_tiles;
    uint16_t tile_amount, collider_amount, npc_amount;

    int add_tile(QPoint position, const AssetData& tile_data);

    int add_collider(QPoint position, const AssetData& collider_data);

public:
    MapData();

    int add_asset(QPoint position, const AssetData& asset_data);

    bool erase_asset(int asset_id);

    void clear_all();
};


#endif  // MAP_DATA_H
