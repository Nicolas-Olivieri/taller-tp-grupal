#include "editor_config.h"

#include "asset_parser.h"
#include "toml.hpp"

#define ASSETS_PATH "/assets_info.toml"

EditorConfig::EditorConfig() {
    const auto data = toml::parse(CONFIG_PATH ASSETS_PATH);
    const std::vector<std::string> categories = {"tiles", "colliders", "npcs"};

    for (const std::string& category: categories) {
        auto assets = toml::find<std::vector<AssetData>>(data, category);
        category_to_assets_data.insert({category, assets});
    }
}

EditorConfig& EditorConfig::get() {
    static EditorConfig instance;
    return instance;
}

std::vector<AssetData> EditorConfig::get_assets_data(const std::string& category) {
    assert(category_to_assets_data.contains(category));
    return category_to_assets_data.at(category);
}
