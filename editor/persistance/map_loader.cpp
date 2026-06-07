#include "map_loader.h"

#include <QFile>
#include <QGraphicsItem>

#define HEADER 0xFAF4

MapLoader::MapLoader(MapData& data, MapCanvas& canvas, QHash<uint8_t, AssetData>& tiles,
                     QHash<uint8_t, AssetData>& colliders, QHash<uint8_t, AssetData>& npcs):
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

    // Cargo todos los items
    load_assets(stream, tiles);
    load_assets(stream, colliders);
    load_assets(stream, npcs);

    file.close();
    return true;
}


void MapLoader::load_assets(QDataStream& stream, const QHash<uint8_t, AssetData>& lookup_assets_hash) const {
    uint16_t assets_amount;

    stream >> assets_amount;

    for (uint16_t i = 0; i < assets_amount; i++) {
        uint8_t id;
        uint16_t origin_x, origin_y;
        stream >> id >> origin_x >> origin_y;

        AssetData asset = lookup_assets_hash[id];
        const auto origin = QPoint(origin_x, origin_y);

        const int placement_id = data.add_asset(origin, asset);
        canvas.set_selected_asset(asset);
        canvas.add_asset_to_scene(origin, placement_id);
    }
}
