#include "client_config.h"

#include <algorithm>
#include <string>

#include <toml.hpp>

#include "common/util/string_utils.h"

#define CLIENT_ITEMS_PATH "/client/items.toml"
#define CLIENT_CREATURES_PATH "/client/creatures.toml"
#define CLIENT_CONSTANTS_PATH "/client/game_constants.toml"
#define CLIENT_UI_DATA_PATH "/client/user_interface.toml"

ClientConfig::ClientConfig() {
    load_from_file(CONFIG_PATH CLIENT_ITEMS_PATH);
    load_from_file(CONFIG_PATH CLIENT_CREATURES_PATH);
    load_from_file(CONFIG_PATH CLIENT_CONSTANTS_PATH);
    load_from_file(CONFIG_PATH CLIENT_UI_DATA_PATH);
}


ClientConfig& ClientConfig::get() {
    static ClientConfig instance;
    return instance;
}


const CreatureDisplayData& ClientConfig::get_creature_data(const uint8_t creature) const {
    if (not creatures_data.contains(creature)) {
        throw std::out_of_range(
                "ClientConfig recibió una criatura que no se encuentra en la configuración del cliente: " +
                std::to_string(creature));
    }

    return creatures_data.at(creature);
}


std::string ClientConfig::get_creature_name(const uint8_t creature) const {
    // Si llega un ID que el cliente no conoce, se muestra un error amigable en lugar de estallar
    if (not creatures_data.contains(creature)) {
        return "???";
    }

    return creatures_data.at(creature).name;
}


const ItemDisplayData& ClientConfig::get_item_data(const uint8_t item_id) const {
    if (not items_data.contains(item_id)) {
        throw std::out_of_range(
                "ClientConfig recibió un ítem que no se encuentra en la configuración del cliente: " +
                std::to_string(item_id));
    }

    return items_data.at(item_id);
}


std::string ClientConfig::get_item_name(const uint8_t item_id) const {
    // Si llega un ID que el cliente no conoce, se muestra un error amigable en el chat en lugar de estallar
    if (not items_data.contains(item_id)) {
        return "Objeto desconocido (" + std::to_string(item_id) + ")";
    }

    return items_data.at(item_id).name;
}


std::optional<uint8_t> ClientConfig::get_item_id(const std::string& item_name) const {
    const std::string target_name = string_utils::to_lowercase(item_name);
    for (const auto& [id, data]: items_data) {
        if (string_utils::to_lowercase(data.name) == target_name) {
            return id;
        }
    }

    return std::nullopt;
}


std::string ClientConfig::get_item_icon_path(const uint8_t item_id) {
    if (not items_data.contains(item_id)) {
        return "resources/items/0.png";  // TODO: Agregar una textura de error
    }

    return items_data.at(item_id).icon_path;
}


void ClientConfig::load_from_file(const std::string& filepath) {
    auto root = toml::parse(filepath);
    if (root.contains("items")) {
        parse_items_table(toml::find(root, "items"));
    }
    if (root.contains("creatures")) {
        parse_creatures_table(toml::find(root, "creatures"));
    }
    if (root.contains("constants")) {
        parse_constants(toml::find(root, "constants"));
    }
    if (root.contains("ui")) {
        parse_ui(toml::find(root, "ui"));
    }
}

void ClientConfig::parse_constants(const toml::value& constants_table) {
    for (const auto& [key, value]: constants_table.as_table()) {
        if (key == "render") {
            render_data = {
                    toml::find<uint8_t>(value, "fps"),
                    toml::find<uint16_t>(value, "screen_width"),
                    toml::find<uint16_t>(value, "screen_height"),
                    toml::find<uint16_t>(value, "tile_size"),
            };
            continue;
        }

        if (key == "sprites") {
            sprite_data = {
                    toml::find<uint8_t>(value, "ghost_head_id"),
                    toml::find<uint8_t>(value, "ghost_body_id"),
                    toml::find<int>(value, "head_offset"),
            };
            continue;
        }

        if (key == "movement") {
            movement_data = {toml::find<uint8_t>(value, "min_pixels_per_step"),
                             toml::find<float>(value, "change_rate")};
            continue;
        }

        throw std::runtime_error(
                std::format("ClientConfig encontró un tipo de constante desconocido: {}", key));
    }
}

void ClientConfig::parse_items_table(const toml::value& items_table) {
    for (const auto& [key, value]: items_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));
        items_data[id] = build_item_display_data(value);
    }
}


ItemDisplayData ClientConfig::build_item_display_data(const toml::value& item_toml) const {
    // TODO: Cargar el resto de atributos de un ítem para el cliente
    return ItemDisplayData(toml::find<std::string>(item_toml, "name"),
                           toml::find<std::string>(item_toml, "icon_path"));
}


void ClientConfig::parse_creatures_table(const toml::basic_value<toml::type_config>& creatures_table) {
    for (const auto& [key, value]: creatures_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));
        creatures_data[id] = build_creature_display_data(value);
    }
}


CreatureDisplayData ClientConfig::build_creature_display_data(const toml::value& creature_toml) const {
    return CreatureDisplayData(toml::find<std::string>(creature_toml, "name"));
}

uint8_t ClientConfig::get_ghost_head_id() const { return sprite_data.ghost_head_id; }

uint8_t ClientConfig::get_ghost_body_id() const { return sprite_data.ghost_body_id; }

int ClientConfig::get_head_offset() const { return sprite_data.head_offset; }

uint8_t ClientConfig::get_fps() const { return render_data.fps; }

uint16_t ClientConfig::get_screen_w() const { return render_data.screen_w; }

uint16_t ClientConfig::get_screen_h() const { return render_data.screen_h; }

uint16_t ClientConfig::get_tile_size() const { return render_data.tile_size; }

const MovementData& ClientConfig::get_movement_data() const { return movement_data; }

void ClientConfig::parse_ui(const toml::value& ui_table) {
    ui_data.history_messages = parse_rect(ui_table, "chat", "history_messages");
    ui_data.input_box = parse_rect(ui_table, "chat", "input_box");

    ui_data.username = parse_rect(ui_table, "player", "username");
    ui_data.clan = parse_rect(ui_table, "player", "clan");
    ui_data.founder = parse_rect(ui_table, "player", "founder_icon");

    ui_data.inventory_title = parse_rect(ui_table, "inventory", "title");
    ui_data.inventory_slots = parse_rect_vector(ui_table, "inventory", "slots");

    ui_data.equipment_slots = parse_rect_vector(ui_table, "equipment", "slots");

    ui_data.stats_title = parse_rect(ui_table, "stats", "title");
    ui_data.health = parse_rect(ui_table, "stats", "health_bar");
    ui_data.mana = parse_rect(ui_table, "stats", "mana_bar");
    ui_data.xp = parse_rect(ui_table, "stats", "xp_bar");

    ui_data.safe_gold = parse_rect(ui_table, "stats", "safe_gold");
    ui_data.excess_gold = parse_rect(ui_table, "stats", "excess_gold");
    ui_data.xp_level = parse_rect(ui_table, "stats", "xp_level");
}

SDL2pp::Rect ClientConfig::parse_rect(const toml::value& config, const std::string& section,
                                      const std::string& key) {
    auto values = toml::find<std::vector<int>>(config, section, key);

    return SDL2pp::Rect(values[0], values[1], values[2], values[3]);
}

std::vector<SDL2pp::Rect> ClientConfig::parse_rect_vector(const toml::value& config,
                                                          const std::string& section,
                                                          const std::string& key) {
    auto rects = toml::find<std::vector<std::vector<int>>>(config, section, key);

    std::vector<SDL2pp::Rect> result;
    result.reserve(rects.size());

    std::transform(rects.begin(), rects.end(), std::back_inserter(result), [](const std::vector<int>& rect) {
        return SDL2pp::Rect(rect[0], rect[1], rect[2], rect[3]);
    });

    return result;
}

const UserInterfaceData& ClientConfig::get_ui_data() const { return ui_data; }
