#include "editor_config.h"

#include "asset_parser.h"
#include "toml.hpp"

#define EDITOR_ASSETS_PATH "/assets_info.toml"
#define EDITOR_CONSTANTS_PATH "/editor_constants.toml"
#define EDITOR_SHORTCUTS_PATH "/editor_shortcuts.toml"

EditorConfig::EditorConfig() {
    load_asset_data();
    load_constants();
    load_shortcuts();
}

void EditorConfig::load_asset_data() {
    const auto data = toml::parse(CONFIG_PATH EDITOR_ASSETS_PATH);
    const std::vector<std::string> categories = {"tiles", "colliders", "npcs"};

    for (const std::string& category: categories) {
        auto assets = toml::find<std::vector<AssetData>>(data, category);
        category_to_assets_data.insert({category, assets});
    }
}

void EditorConfig::load_constants() {
    const auto constants_data = toml::parse(CONFIG_PATH EDITOR_CONSTANTS_PATH);

    const SafeZoneData safe_zone_data{toml::find<uint8_t>(constants_data, "safe_zone_id"),
                                      toml::find<uint8_t>(constants_data, "safe_zone_brush_w"),
                                      toml::find<uint8_t>(constants_data, "safe_zone_brush_h")};

    const uint16_t tile_size = toml::find<uint16_t>(constants_data, "tile_size");
    const uint16_t file_header = toml::find<uint16_t>(constants_data, "file_header");


    constants = {safe_zone_data, file_header, tile_size};
}

EditorConfig& EditorConfig::get() {
    static EditorConfig instance;
    return instance;
}

std::vector<AssetData> EditorConfig::get_assets_data(const std::string& category) const {
    assert(category_to_assets_data.contains(category));
    return category_to_assets_data.at(category);
}

void EditorConfig::load_shortcuts() {
    const auto shortcuts_data = toml::parse(CONFIG_PATH EDITOR_SHORTCUTS_PATH);

    shortcuts = {toml::find<std::string>(shortcuts_data, "draw_key"),
                 toml::find<std::string>(shortcuts_data, "drag_key"),
                 toml::find<std::string>(shortcuts_data, "erase_key"),
                 toml::find<std::string>(shortcuts_data, "safe_zone_key")};
}

uint16_t EditorConfig::get_tile_size() const { return constants.tile_size; }

uint16_t EditorConfig::get_file_header() const { return constants.file_header; }

const SafeZoneData& EditorConfig::get_safe_zone_data() const { return constants.safe_zone; }

const ShortcutsKeys& EditorConfig::get_shortcuts_keys() const { return shortcuts; }
