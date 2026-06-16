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

    uint8_t server_start;
    qint64 server_end;
    stream >> server_start >> server_end;

    uint16_t width, height;
    stream >> width >> height;

    // Cargo todos los items
    file.seek(server_end);
    load_assets(stream, tiles);
    load_assets(stream, colliders);

    // Cargados los items, cargo las zonas seguras (dependen de si hay tiles colocadas, se debe hacer al
    // final), los npcs y los teleports que se guardan para el servidor
    file.seek(server_start);
    load_safe_zone(stream, width, height);
    load_assets(stream, npcs);
    load_teleports(stream);

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

void MapLoader::load_teleports(QDataStream& stream) const {
    // Al guardarse los npcs automaticamente, es muy probable que se guarden mal. De esta forma se guardan en
    // orden.
    data.teleport_pairs.clear();

    uint16_t teleports_amount;
    stream >> teleports_amount;

    for (uint16_t i = 0; i < teleports_amount; i++) {
        uint16_t port_a_x, port_a_y, port_b_x, port_b_y;
        stream >> port_a_x >> port_a_y >> port_b_x >> port_b_y;

        const auto base_a = QPoint(port_a_x, port_a_y - 1);
        const auto base_b = QPoint(port_b_x, port_b_y - 1);

        int a_id = data.occupied_tiles[base_a].last();
        int b_id = data.occupied_tiles[base_b].last();

        data.teleport_pairs.insert({{a_id, b_id}, {b_id, a_id}});
    }
}
