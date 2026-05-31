#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <qpixmap.h>

enum class EditorMode {
    DRAW, ERASE, SELECT, SET_COLLISIONS, NONE
};

enum class ImageType {
    TILE, COLLIDER
};

struct AssetData {
    uint8_t id;
    QPixmap img;
    QString name;
    ImageType type; // TILE o COLLIDER
    int tile_width;
    int tile_height;
    QVector<QVector<bool>> unwalkable_tiles;  // MATRIZ 0 = caminable, 1 = NO caminable
};


struct Placement {
    int id;
    QRect bouds;
    AssetData asset;
};

#endif  // COMPONENTS_H
