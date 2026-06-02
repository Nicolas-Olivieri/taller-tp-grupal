#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QPixmap>

enum class EditorMode { DRAW, ERASE, DRAG };

enum class ImageType { TILE, COLLIDER };

struct AssetData {
    uint8_t id;
    QPixmap img;
    QString name;
    ImageType type;  // TILE o COLLIDER
    int tile_width;
    int tile_height;
    QRect unwalkable_tiles;
};


struct Placement {
    int id;
    QPoint origin;
    AssetData asset;
};

#endif  // COMPONENTS_H
