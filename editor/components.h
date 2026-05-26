#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <qpixmap.h>


struct Tile {
    uint8_t id;
    QPixmap img;
    QString name; // Definirlo en el TOML para mayor claridad
};

struct CollidableItem {
    uint8_t id;
    QPixmap img;
    int tile_width;
    int tile_height;
    QVector<QVector<bool>> collidable_tiles; // MATRIZ 0 = caminable, 1 = NO caminable
};



#endif //COMPONENTS_H
