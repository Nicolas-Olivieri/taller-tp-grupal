#ifndef FONT_PARSER_H
#define FONT_PARSER_H

#include <string>
#include <unordered_map>

#include <toml.hpp>


enum class FontType {
    LABEL_NAME_LEVEL,
    LABEL_CLAN,
    UI_USERNAME,
    UI_CLAN,
    UI_MENU_TITLE,
    UI_MENU,
    UI_CHAT,
    UI_ITEM_AMOUNT
};


struct FontConfig {
    std::string path;
    int size;
};


struct FontConfigs {
    std::unordered_map<FontType, FontConfig> configs;
};


template <>
struct toml::from<FontConfigs> {
    static FontConfigs from_toml(const toml::value& v) {
        // TODO: Considerar moverlo a TomlHelper
        static const std::unordered_map<std::string, FontType> string_to_font_type({
                {"label_name_level", FontType::LABEL_NAME_LEVEL},
                {"label_clan", FontType::LABEL_CLAN},
                {"ui_username", FontType::UI_USERNAME},
                {"ui_clan", FontType::UI_CLAN},
                {"ui_menu_title", FontType::UI_MENU_TITLE},
                {"ui_menu", FontType::UI_MENU},
                {"ui_chat", FontType::UI_CHAT},
                {"ui_item_amount", FontType::UI_ITEM_AMOUNT},
        });

        FontConfigs config;
        for (const auto& [key, value]: v.as_table()) {
            FontType font_type = string_to_font_type.at(key);

            auto raw_path = toml::find<std::string>(value, "path");
            const int size = toml::find<int>(value, "size");

            const std::string full_path = std::string(DATA_PATH) + raw_path;

            config.configs[font_type] = FontConfig{full_path, size};
        }

        return config;
    }
};


#endif  // FONT_PARSER_H
