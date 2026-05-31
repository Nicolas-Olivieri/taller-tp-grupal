#ifndef EDITOR_MAP_H
#define EDITOR_MAP_H
#include <qhash.h>
#include <qpoint.h>
#include <qset.h>

#include "components.h"


class EditorMap {
private:
    int tile_id;
    QHash<int, Placement> placements;
    QHash<QPoint, QVector<int>> occupied_tiles;

public:
    EditorMap();

    bool add_tile(QPoint position, const AssetData &tile_data);

    bool add_collider(QPoint position, const AssetData &collider_data);
};


#endif //EDITOR_MAP_H
