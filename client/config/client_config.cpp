#include "client_config.h"

#include <string>

#include <toml.hpp>

#include "common/util/string_utils.h"

#define CLIENT_ITEMS_PATH "/client/items.toml"
#define CLIENT_CREATURES_PATH "/client/creatures.toml"


ClientConfig::ClientConfig() {
    loadFromFile(std::string(DATA_PATH) + std::string(CLIENT_ITEMS_PATH));
    loadFromFile(std::string(DATA_PATH) + std::string(CLIENT_CREATURES_PATH));
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
