#ifndef EDITOR_MAP_H
#define EDITOR_MAP_H
#include <qhash.h>
#include <qpoint.h>

#include "components.h"


class EditorMap {
private:
    int tile_id;
    QHash<int, Placement> placements;
    QHash<QPoint, QVector<int>> occupied_tiles;

    int add_tile(QPoint position, const AssetData &tile_data);

    int add_collider(QPoint position, const AssetData &collider_data);

public:
    EditorMap();

    int add_asset(QPoint position, const AssetData &asset_data);

    bool erase_asset(int asset_id);
};


#endif //EDITOR_MAP_H
