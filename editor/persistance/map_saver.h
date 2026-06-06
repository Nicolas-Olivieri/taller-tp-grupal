#ifndef MAP_SAVER_H
#define MAP_SAVER_H

#include "map_data.h"

class MapSaver {
private:
    MapData& data;
    QPoint min_point;
    QSize matrix_size;

    void get_origin_and_matrix_size();

    void store_offset_and_dimensions_data(QDataStream& stream) const;

    void store_server_data(QDataStream& stream) const;

    void store_client_data(QDataStream& stream) const;

    void store_asset_data(QDataStream& stream, ImageType type) const;

public:
    explicit MapSaver(MapData& data);

    void save(const QString& user_filename);

    QString format_filename(const QString& filename);
};


#endif  // MAP_SAVER_H
