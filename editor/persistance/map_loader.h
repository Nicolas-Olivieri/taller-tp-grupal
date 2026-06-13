#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <QHash>

#include "../components.h"
#include "../map_data.h"
#include "../view/map_canvas.h"


class MapLoader {
private:
    MapData& data;
    MapCanvas& canvas;

    QHash<uint8_t, AssetData>& tiles;
    QHash<uint8_t, AssetData>& colliders;
    QHash<uint8_t, AssetData>& npcs;

    void load_assets(QDataStream &stream, const QHash<uint8_t, AssetData> &lookup_assets_hash) const;

public:
    MapLoader(MapData& data, MapCanvas& canvas, QHash<uint8_t, AssetData>& tiles,
              QHash<uint8_t, AssetData>& colliders, QHash<uint8_t, AssetData>& npcs);

    bool load(const QString& filename) const;
};


#endif  // MAP_LOADER_H
