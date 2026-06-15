#include "client_config.h"

#include <string>

#include <toml.hpp>

#include "common/util/string_utils.h"

#define CLIENT_ITEMS_PATH "/client/items.toml"
#define CLIENT_CREATURES_PATH "/client/creatures.toml"
#define CLIENT_CONSTANTS_PATH "/client/game_constants.toml"


ClientConfig::ClientConfig() {
    loadFromFile(CONFIG_PATH CLIENT_ITEMS_PATH);
    loadFromFile(CONFIG_PATH CLIENT_CREATURES_PATH);
    loadFromFile(CONFIG_PATH CLIENT_CONSTANTS_PATH);
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


void ClientConfig::loadFromFile(const std::string& filepath) {
    auto root = toml::parse(filepath);
    if (root.contains("items")) {
        parseItemsTable(toml::find(root, "items"));
    }
    if (root.contains("creatures")) {
        parseCreaturesTable(toml::find(root, "creatures"));
    }
    if (root.contains("constants")) {
        parse_constants(toml::find(root, "constants"));
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

void ClientConfig::parseItemsTable(const toml::value& items_table) {
    for (const auto& [key, value]: items_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));
        items_data[id] = buildItemDisplayData(value);
    }
}


ItemDisplayData ClientConfig::buildItemDisplayData(const toml::value& item_toml) const {
    // TODO: Cargar el resto de atributos de un ítem para el cliente
    return ItemDisplayData(toml::find<std::string>(item_toml, "name"),
                           toml::find<std::string>(item_toml, "icon_path"));
}


void ClientConfig::parseCreaturesTable(const toml::basic_value<toml::type_config>& creatures_table) {
    for (const auto& [key, value]: creatures_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));
        creatures_data[id] = buildCreatureDisplayData(value);
    }
}


CreatureDisplayData ClientConfig::buildCreatureDisplayData(const toml::value& creature_toml) const {
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
