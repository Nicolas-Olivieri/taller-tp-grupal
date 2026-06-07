#ifndef ASSET_PARSER_H
#define ASSET_PARSER_H

#include <string>
#include <vector>

#include <toml.hpp>

#include "components.h"

template <>
struct toml::from<AssetData> {
    static AssetData from_toml(const value& v) {
        AssetData data;
        data.id = toml::find<uint8_t>(v, "id");
        data.name = QString::fromStdString(toml::find<std::string>(v, "name"));
        data.tile_width = toml::find<uint16_t>(v, "width");
        data.tile_height = toml::find<uint16_t>(v, "height");

        const auto rect_values = toml::find<std::vector<int>>(v, "unwalkable");
        data.unwalkable_area = QRect(rect_values[0], rect_values[1], rect_values[2], rect_values[3]);

        data.type = ImageType::TILE;  // Luego se asigna de forma correcta

        return data;
    }
};

#endif  // ASSET_PARSER_H
