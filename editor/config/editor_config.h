#ifndef EDITOR_CONFIG_H
#define EDITOR_CONFIG_H

#include <string>
#include <unordered_map>
#include <vector>

#include "components.h"
#include "toml.hpp"

struct SafeZoneData {
    uint8_t id;
    uint8_t brush_w;
    uint8_t brush_h;
};

struct EditorConstantsData {
    SafeZoneData safe_zone;
    uint16_t file_header;
    uint16_t tile_size;
};

struct ShortcutsKeys {
    std::string draw_key;
    std::string drag_key;
    std::string erase_key;
    std::string safe_zone_key;
};

class EditorConfig {
private:
    std::unordered_map<std::string, std::vector<AssetData>> category_to_assets_data;
    EditorConstantsData constants;
    ShortcutsKeys shortcuts;

    EditorConfig();

    void load_asset_data();

    void load_constants();

    void load_shortcuts();

public:
    static EditorConfig& get();

    std::vector<AssetData> get_assets_data(const std::string& category) const;

    uint16_t get_tile_size() const;

    uint16_t get_file_header() const;

    const SafeZoneData& get_safe_zone_data() const;

    const ShortcutsKeys& get_shortcuts_keys() const;
};


#endif  // EDITOR_CONFIG_H
