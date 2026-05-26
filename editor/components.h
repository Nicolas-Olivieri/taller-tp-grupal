#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <qpixmap.h>


struct TileView {
    uint8_t id;
    QPixmap img;
    QString name;  // Definirlo en el TOML para mayor claridad
    bool is_walkable;
};

struct CollidableView {
    uint8_t id;
    QPixmap img;
    QString name;
    int tile_width;
    int tile_height;
    QVector<QVector<bool>> collidable_tiles;  // MATRIZ 0 = caminable, 1 = NO caminable
};


#endif  // COMPONENTS_H
