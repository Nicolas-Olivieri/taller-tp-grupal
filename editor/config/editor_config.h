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
    uint8_t teleport_id;
};

struct ShortcutsKeys {
    std::string draw_key;
    std::string drag_key;
    std::string erase_key;
    std::string safe_zone_key;
};

struct TutorialData {
    std::string drag_btn;
    std::string draw_btn;
    std::string erase_btn;
    std::string safe_zone_btn;
    std::string unwalkable_cbox;
    std::string safe_zone_cbox;
    std::string asset_info;
    std::string load_btn;
    std::string save_btn;
};

class EditorConfig {
private:
    std::unordered_map<std::string, std::vector<AssetData>> category_to_assets_data;
    EditorConstantsData constants;
    ShortcutsKeys shortcuts;
    TutorialData tutorial;

    EditorConfig();

    void load_asset_data();

    void load_constants();

    void load_tutorial_data();

    void load_shortcuts();

public:
    static EditorConfig& get();

    std::vector<AssetData> get_assets_data(const std::string& category) const;

    uint16_t get_tile_size() const;

    uint16_t get_file_header() const;

    const SafeZoneData& get_safe_zone_data() const;

    const ShortcutsKeys& get_shortcuts_keys() const;

    const TutorialData& get_tutorial_data() const;

    uint8_t get_teleport_id() const;
};


#endif  // EDITOR_CONFIG_H
