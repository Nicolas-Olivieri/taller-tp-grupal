#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <qhash.h>
#include <qpoint.h>
#include <QSet>

#include "components.h"


class MapData {
    friend class MapSaver;
    friend class MapLoader;

private:
    int tile_id;
    QHash<int, Placement> placements;
    QHash<QPoint, QVector<int>> occupied_tiles;
    QHash<QPoint, QVector<int>> unwalkable_tiles;
    QSet<QPoint> safe_zone_tiles;

    QHash<ImageType, uint16_t> asset_counter;

    int add_tile(QPoint position, const AssetData& tile_data);

    int add_collider(QPoint position, const AssetData& collider_data);

    bool erase_tile(const Placement &placement);

    bool erase_collider(const Placement &placement);

public:
    MapData();

    int add_asset(QPoint position, const AssetData& asset_data);

    bool erase_asset(int asset_id);

    QSet<QPair<int, QPoint>> add_safe_tiles(QPoint origin_position, int width, int height);

    void erase_safe_tile(QPoint position);

    void clear_all();
};


#endif  // MAP_DATA_H
