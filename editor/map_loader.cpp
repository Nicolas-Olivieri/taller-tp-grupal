#include "map_loader.h"

#include <QFile>
#include <QGraphicsItem>

MapLoader::MapLoader(MapData& data, MapCanvas& canvas,
                     QHash<uint8_t, AssetData> &tiles, QHash<uint16_t, AssetData>& colliders) :
    data(data), canvas(canvas), tiles(tiles), colliders(colliders) {}


void MapLoader::load(const QString& filename) const {
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);

    // Borro la información anterior
    reset_editor();

    // Skipeo hasta la información que sirve al cliente, que es la que sirve al editor
    uint16_t server_start, server_end;
    stream >> server_start >> server_end;
    file.seek(server_end);

    // Cargo los tiles
    load_assets<uint8_t>(stream, tiles);

    // Cargo los colliders
    load_assets<uint16_t>(stream, colliders);

    file.close();
}


void MapLoader::reset_editor() const {
    data.occupied_tiles.clear();
    data.unwalkable_tiles.clear();
    data.placements.clear();

    canvas.erase_all_assets();
}

template <typename intType>
void MapLoader::load_assets(QDataStream& stream, QHash<intType, AssetData>& lookup_assets_hash) const {
    uint16_t assets_amount;

    stream >> assets_amount;
    for (uint16_t i = 0; i < assets_amount; i++) {
        intType id;
        uint16_t origin_x, origin_y;
        stream >> id >> origin_x >> origin_y;

        AssetData asset = lookup_assets_hash[id];
        const auto origin = QPoint(origin_x, origin_y);

        const int placement_id = data.add_asset(origin, asset);
        canvas.set_selected_asset(asset);
        canvas.add_asset_to_scene(origin, placement_id);
    }
}
