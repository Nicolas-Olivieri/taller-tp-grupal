#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include <toml11/types.hpp>

struct CreatureDisplayData {
    std::string name;
};

struct ItemDisplayData {
    // TODO: Agregar el resto de atributos de un ítem para el cliente
    std::string name;
    std::string icon_path;
};

struct RenderData {
    uint8_t fps;
    uint16_t screen_w;
    uint16_t screen_h;
    uint16_t tile_size;
};

struct SpriteData {
    uint8_t ghost_head_id;
    uint8_t ghost_body_id;
    int head_offset;
};

class ClientConfig {
private:
    std::unordered_map<uint8_t, CreatureDisplayData> creatures_data;
    std::unordered_map<uint8_t, ItemDisplayData> items_data;
    RenderData render_data;
    SpriteData sprite_data;

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

private:
    ClientConfig();

    void loadFromFile(const std::string& filepath);

    void parseItemsTable(const toml::value& items_table);

    ItemDisplayData buildItemDisplayData(const toml::value& item_toml) const;

    void parseCreaturesTable(const toml::basic_value<toml::type_config>& creatures_table);

    CreatureDisplayData buildCreatureDisplayData(const toml::value& creature_toml) const;

    void parse_constants(const toml::value& constants_table);
};


#endif  // CLIENT_CONFIG_H
