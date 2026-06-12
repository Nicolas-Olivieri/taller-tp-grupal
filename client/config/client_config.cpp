#include "client_config.h"

#include <string>

#include <toml.hpp>

#include "common/util/string_utils.h"

#define CLIENT_ITEMS_PATH "/client/items.toml"


ClientConfig::ClientConfig() { loadFromFile(std::string(CONFIG_PATH) + CLIENT_ITEMS_PATH); }


ClientConfig& ClientConfig::get() {
    static ClientConfig instance;
    return instance;
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
}


void ClientConfig::parseItemsTable(const toml::value& items_table) {
    for (const auto& [key, value]: items_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));
        items_data[id] = buildItemDisplayData(value);
    }
}


ItemDisplayData ClientConfig::buildItemDisplayData(const toml::value& item_toml) {
    // TODO: Cargar el resto de atributos de un ítem para el cliente
    return ItemDisplayData(toml::find<std::string>(item_toml, "name"),
                           toml::find<std::string>(item_toml, "icon_path"));
}
