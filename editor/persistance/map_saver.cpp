#include "map_saver.h"

#include <QFileDialog>
#include <iostream>

#define WALKABLE 1
#define UNWALKABLE 0
#define HEADER 0xFAF4

MapSaver::MapSaver(MapData& data): data(data) {}

/*
 * El formato en que guarda los datos es:
 * <offset inicio server uint16_t> <offset fin server uint16_t>
 * <width en tiles del mundo uint16_t> <height en tiles del mundo uint16_t>
 *
 * <matriz fila por fila indicando si cada tile es walkable o unwalkable uint8_t>
 *
 * <cant tiles uint16_t> [<id uint8_t> <origen_x uint16_t> <origen_y uint16_t>] [] ...
 * <cant colliders uint16_t> [<id uint16_t> <origen_x uint16_t> <origen_y uint16_t>] [] ...
 */
void MapSaver::save(const QString& user_filename) {
    const QString filename = format_filename(user_filename);

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QDataStream stream(&file);

    get_origin_and_matrix_size();
    store_offset_and_dimensions_data(stream);
    store_server_data(stream);
    store_client_data(stream);

    file.close();
}

QString MapSaver::format_filename(const QString& filename) {
    const QFileInfo file_info(filename);
    return file_info.path() + "/" + file_info.completeBaseName() + ".bin";
}

void MapSaver::get_origin_and_matrix_size() {
    QList<QPoint> base_points = data.occupied_tiles.keys();

    const auto min_max = std::minmax_element(
            base_points.begin(), base_points.end(),
            [](const QPoint& a, const QPoint& b) { return a.x() + a.y() < b.x() + b.y(); });

    const QPoint min = *min_max.first;
    const QPoint max = *min_max.second;

    min_point = min;
    matrix_size = QSize(max.x() - min.x() + 1, max.y() - min.y() + 1);
}

void MapSaver::store_offset_and_dimensions_data(QDataStream& stream) const {
    const uint16_t world_width = matrix_size.width();
    const uint16_t world_height = matrix_size.height();
    constexpr uint16_t header = HEADER;

    const uint16_t server_start =
            sizeof(header) + sizeof(uint16_t) * 2 + sizeof(world_width) + sizeof(world_height);

    const uint16_t server_end = server_start + sizeof(uint8_t) * world_width * world_height;

    stream << header << server_start << server_end << world_width << world_height;
}

void MapSaver::store_server_data(QDataStream& stream) const {
    for (int y = 0; y < matrix_size.height(); y++) {
        for (int x = 0; x < matrix_size.width(); x++) {
            uint8_t is_walkable;
            auto translated_point = QPoint(min_point.x() + x, min_point.y() + y);

            if (data.unwalkable_tiles.contains(translated_point) ||
                !data.occupied_tiles.contains(translated_point)) {
                is_walkable = UNWALKABLE;
            } else {
                is_walkable = WALKABLE;
            }

            stream << is_walkable;
        }
    }
}

void MapSaver::store_client_data(QDataStream& stream) const {
    uint16_t tile_amount = 0;
    uint16_t collider_amount = 0;

    QByteArray tile_data;
    QByteArray collider_data;
    QDataStream tile_stream(&tile_data, QIODevice::WriteOnly);
    QDataStream collider_stream(&collider_data, QIODevice::WriteOnly);

    for (const auto& placement: data.placements) {
        const uint16_t origin_x = placement.origin.x() - min_point.x();
        const uint16_t origin_y = placement.origin.y() - min_point.y();

        if (placement.asset.type == ImageType::TILE) {
            const uint8_t id = placement.asset.id;
            tile_stream << id << origin_x << origin_y;
            tile_amount++;

        } else if (placement.asset.type == ImageType::COLLIDER) {
            const uint16_t id = placement.asset.id;
            collider_stream << id << origin_x << origin_y;
            collider_amount++;
        }
    }

    stream << tile_amount;
    stream.writeRawData(tile_data.constData(), tile_data.size());
    stream << collider_amount;
    stream.writeRawData(collider_data.constData(), collider_data.size());
}
