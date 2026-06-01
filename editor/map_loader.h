#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include "components.h"
#include <QHash>

#include "map_canvas.h"
#include "map_data.h"


class MapLoader {
private:
    MapData& data;
    MapCanvas& canvas;

    QHash<uint8_t, AssetData>& tiles;
    QHash<uint16_t, AssetData>& colliders;

    void reset_editor() const;

    template<class intType>
    void load_assets(QDataStream &stream, QHash<intType, AssetData>& lookup_assets_hash) const;

public:
    MapLoader(MapData& data, MapCanvas& canvas, QHash<uint8_t, AssetData>& tiles, QHash<uint16_t, AssetData>& colliders);

    void load(const QString& filename) const;
};


#endif //MAP_LOADER_H
