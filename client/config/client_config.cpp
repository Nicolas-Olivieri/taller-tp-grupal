#include "client_config.h"

#include <string>

#include <toml.hpp>

#define CLIENT_ITEMS_PATH "/client/items.toml"


ClientConfig::ClientConfig() { loadFromFile(std::string(DATA_PATH) + CLIENT_ITEMS_PATH); }


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
    return ItemDisplayData(toml::find<std::string>(item_toml, "name"));
}
