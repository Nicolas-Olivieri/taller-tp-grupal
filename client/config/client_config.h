#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <toml.hpp>

#include "SDL2pp/Rect.hh"

#include "client_data.h"

class ClientConfig {
private:
    std::unordered_map<uint8_t, CreatureDisplayData> creatures_data;
    std::unordered_map<uint8_t, ItemDisplayData> items_data;
    RenderData render_data;
    SpriteData sprite_data;
    MovementData movement_data;
    UserInterfaceData ui_data;

public:
    static ClientConfig& get();

    ClientConfig(const ClientConfig&) = delete;

    ClientConfig& operator=(const ClientConfig&) = delete;

    const CreatureDisplayData& get_creature_data(uint8_t creature) const;

    std::string get_creature_name(uint8_t creature) const;

    const ItemDisplayData& get_item_data(uint8_t item_id) const;

    std::string get_item_name(uint8_t item_id) const;

    std::optional<uint8_t> get_item_id(const std::string& item_name) const;

    std::string get_item_icon_path(uint8_t item_id);

    uint8_t get_ghost_head_id() const;

    uint8_t get_ghost_body_id() const;

    int get_head_offset() const;

    uint8_t get_fps() const;

    uint16_t get_screen_w() const;

    uint16_t get_screen_h() const;

    uint16_t get_tile_size() const;

    const MovementData& get_movement_data() const;

    const UserInterfaceData& get_ui_data() const;

private:
    ClientConfig();

    void load_from_file(const std::string& filepath);

    void parse_items_table(const toml::value& items_table);

    ItemDisplayData build_item_display_data(const toml::value& item_toml) const;

    void parse_creatures_table(const toml::basic_value<toml::type_config>& creatures_table);

    CreatureDisplayData build_creature_display_data(const toml::value& creature_toml) const;

    void parse_constants(const toml::value& constants_table);

    void parse_ui(const toml::value& ui_table);

    SDL2pp::Rect parse_rect(const toml::value& config, const std::string& section, const std::string& key);

    std::vector<SDL2pp::Rect> parse_rect_vector(const toml::value& config, const std::string& section,
                                                const std::string& key);
};


#endif  // CLIENT_CONFIG_H
