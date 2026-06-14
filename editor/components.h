#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QPixmap>
#include "editor_constants.h"

struct AssetData {
    uint8_t id;
    QPixmap img;
    QString name;
    ImageType type;  // TILE o COLLIDER o NPC
    int tile_width;
    int tile_height;
    QRect unwalkable_area;
    bool inverse_unwalkable;
};


struct Placement {
    int id;
    QPoint origin;
    AssetData asset;
};

#endif  // COMPONENTS_H
