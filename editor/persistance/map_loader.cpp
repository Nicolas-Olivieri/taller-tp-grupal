#include "map_loader.h"

#include <QFile>
#include <QGraphicsItem>

#define HEADER 0xFAF4

MapLoader::MapLoader(MapData& data, MapCanvas& canvas, QHash<uint8_t, AssetData>& tiles,
                     QHash<uint16_t, AssetData>& colliders, QHash<uint8_t, AssetData>& npcs):
        data(data), canvas(canvas), tiles(tiles), colliders(colliders), npcs(npcs) {}


bool MapLoader::load(const QString& filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QDataStream stream(&file);

    uint16_t header = 0;
    stream >> header;
    if (stream.status() != QDataStream::Ok || header != static_cast<uint16_t>(HEADER)) {
        return false;
    }

    // Skipeo hasta la información que sirve al cliente, que es la que sirve al editor
    uint16_t server_start, server_end;
    stream >> server_start >> server_end;
    file.seek(server_end);

    // Cargo los tiles
    load_assets<uint8_t>(stream, tiles);

    // Cargo los colliders
    load_assets<uint16_t>(stream, colliders);

    // Cargo los npcs
    load_assets<uint8_t>(stream, npcs);

    file.close();
    return true;
}


template <typename intType>
void MapLoader::load_assets(QDataStream& stream, const QHash<intType, AssetData>& lookup_assets_hash) const {
    uint16_t assets_amount;

    stream >> assets_amount;
    for (uint16_t i = 0; i < assets_amount; i++) {
        intType raw_id;
        uint16_t origin_x, origin_y;
        stream >> raw_id >> origin_x >> origin_y;

        auto id = static_cast<uint16_t>(raw_id);
        AssetData asset = lookup_assets_hash[id];
        const auto origin = QPoint(origin_x, origin_y);

        const int placement_id = data.add_asset(origin, asset);
        canvas.set_selected_asset(asset);
        canvas.add_asset_to_scene(origin, placement_id);
    }
}
