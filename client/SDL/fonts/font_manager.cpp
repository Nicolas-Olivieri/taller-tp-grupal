#include "font_manager.h"

#include <string>

#define FONTS_PATH "/client/fonts.toml"


FontManager::FontManager(const std::string& resolution) { load_font_config(resolution); }


void FontManager::load_font_config(const std::string& resolution) {
    auto root = toml::parse(std::string(CONFIG_PATH) + std::string(FONTS_PATH));
    if (root.contains("fonts")) {
        auto [configs] = toml::find<FontConfigs>(root, "fonts", resolution);
        for (const auto& [type, config]: configs) {
            fonts.emplace(type, SDL2pp::Font(config.path, config.size));
        }
    }
}


SDL2pp::Font& FontManager::get_font(const FontType& type) { return fonts.at(type); }
