#include "map_loader.h"

#include <QFile>
#include <QGraphicsItem>

#include "config/editor_config.h"

#include "editor_constants.h"

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
    const auto expected_file_header = EditorConfig::get().get_file_header();
    if (stream.status() != QDataStream::Ok || header != static_cast<uint16_t>(expected_file_header)) {
        return false;
    }

    uint32_t server_start, server_end;
    stream >> server_start >> server_end;

    uint16_t width, height;
    stream >> width >> height;

    // Cargo todos los items
    file.seek(server_end);
    load_assets(stream, tiles);
    load_assets(stream, colliders);
    load_assets(stream, npcs);

    // Cargados los items, cargo las zonas seguras (dependen de si hay tiles colocadas, se debe hacer al
    // final)
    file.seek(server_start);
    load_safe_zone(stream, width, height);

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

void MapLoader::load_safe_zone(QDataStream& stream, const int width, const int height) const {
    const auto tile_size = EditorConfig::get().get_tile_size();
    const auto safe_zone_id = EditorConfig::get().get_safe_zone_data().id;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint8_t walkability;
            uint8_t biome;
            stream >> walkability >> biome;

            if (biome == safe_zone_id) {
                canvas.set_safe_tiles(QPoint(x * tile_size, y * tile_size), 1, 1);
            }
        }
    }
}
