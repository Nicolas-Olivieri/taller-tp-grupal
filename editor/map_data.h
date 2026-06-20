#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <QSet>
#include <set>
#include <vector>

#include <qhash.h>
#include <qpoint.h>

#include "components.h"


class MapData {
    friend class MapSaver;
    friend class MapLoader;

private:
    int tile_id;
    QHash<int, Placement> placements;
    QHash<QPoint, std::vector<int>> occupied_tiles;
    QHash<QPoint, std::set<int>> unwalkable_tiles;
    QSet<QPoint> safe_zone_tiles;

    QHash<ImageType, uint16_t> asset_counter;

    QVector<int> curr_teleport_pair;
    QHash<int, int> teleport_pairs;

    int add_tile(QPoint position, const AssetData& tile_data);
    int add_collider(QPoint position, const AssetData& collider_data);
    void add_teleport(int id);

    bool erase_tile(const Placement& placement);
    bool erase_collider(const Placement& placement);
    void erase_teleport(int point_a);

public:
    MapData();

    int add_asset(QPoint position, const AssetData& asset_data);

    bool erase_asset(int asset_id);

    QSet<QPair<int, QPoint>> add_safe_tiles(QPoint origin_position, int width, int height);

    void erase_safe_tile(QPoint position);

    void clear_all();
};


#endif  // MAP_DATA_H
