#include "map_saver.h"

#include <QFileDialog>
#include <algorithm>
#include <iostream>

#include "grid_range.h"

#define WALKABLE 1
#define UNWALKABLE 0
#define HEADER 0xFAF4

MapSaver::MapSaver(MapData& data): data(data) {}

/*
 * El formato en que guarda los datos es:
 * <offset inicio server uint16_t> <offset fin server uint16_t>
 * <width en tiles del mundo uint16_t> <height en tiles del mundo uint16_t>
 *
 * <matriz fila por fila indicando si cada tile es walkable o unwalkable seguido del id de bioma uint16_t>
 * <cant npcs uint16_t> [<id uint8_t> <origen_x uint16_t> <origen_y uint16_t>] [] ...
 *
 * <cant tiles uint16_t> [<id uint8_t> <origen_x uint16_t> <origen_y uint16_t>] [] ...
 * <cant colliders uint16_t> [<id uint8_t> <origen_x uint16_t> <origen_y uint16_t>] [] ...
 * <cant npcs uint16_t> [<id uint8_t> <origen_x uint16_t> <origen_y uint16_t>] [] ...
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

    if (base_points.empty()) {
        min_point = QPoint(0, 0);
        matrix_size = QSize(0, 0);
        return;
    }

    int min_x = base_points[0].x();
    int min_y = base_points[0].y();
    int max_x = min_x;
    int max_y = min_y;

    for (const auto& point: base_points) {
        min_x = std::min(min_x, point.x());
        min_y = std::min(min_y, point.y());
        max_x = std::max(max_x, point.x());
        max_y = std::max(max_y, point.y());
    }

    min_point = QPoint(min_x, min_y);
    matrix_size = QSize(max_x - min_x + 1, max_y - min_y + 1);
}

void MapSaver::store_offset_and_dimensions_data(QDataStream& stream) const {
    const uint16_t world_width = matrix_size.width();
    const uint16_t world_height = matrix_size.height();
    constexpr uint16_t header = HEADER;
    constexpr size_t npc_data_size = sizeof(uint8_t) + sizeof(uint16_t) * 2;

    constexpr uint16_t server_start =
            sizeof(header) + sizeof(uint16_t) * 2 + sizeof(world_width) + sizeof(world_height);

    const uint16_t server_end = server_start + sizeof(uint16_t) * world_width * world_height +
                                sizeof(uint16_t) + data.asset_counter[ImageType::NPC] * npc_data_size;

    stream << header << server_start << server_end << world_width << world_height;
}

void MapSaver::store_server_data(QDataStream& stream) const {
    GridRange grid_range(min_point, matrix_size.width(), matrix_size.height());

    for (const auto& cell: grid_range) {
        uint8_t is_walkable;
        if (data.unwalkable_tiles.contains(cell) || !data.occupied_tiles.contains(cell)) {
            is_walkable = UNWALKABLE;
        } else {
            is_walkable = WALKABLE;
        }

        uint8_t biome;
        if (data.occupied_tiles.contains(cell)) {
            const uint8_t data_id = data.occupied_tiles[cell][0];
            biome = data.placements[data_id].asset.id;
        } else {
            biome = 0;
        }

        stream << is_walkable << biome;
    }

    store_asset_data(stream, ImageType::NPC);
}

void MapSaver::store_client_data(QDataStream& stream) const {
    store_asset_data(stream, ImageType::TILE);
    store_asset_data(stream, ImageType::COLLIDER);
    store_asset_data(stream, ImageType::NPC);
}

void MapSaver::store_asset_data(QDataStream& stream, const ImageType type) const {
    stream << data.asset_counter[type];

    for (const auto& placement: data.placements) {
        if (placement.asset.type != type) {
            continue;
        }

        const uint16_t origin_x = placement.origin.x() - min_point.x();
        const uint16_t origin_y = placement.origin.y() - min_point.y();

        const uint8_t id = placement.asset.id;
        stream << id << origin_x << origin_y;
    }
}
