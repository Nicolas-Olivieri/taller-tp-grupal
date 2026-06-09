#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include <cstdint>
#include <map>
#include <optional>
#include <string>

#include "toml11/types.hpp"


struct ItemDisplayData {
    // TODO: Agregar el resto de atributos de un ítem para el cliente
    std::string name;
    std::string icon_path;
};


class ClientConfig {
private:
    std::map<uint8_t, ItemDisplayData> items_data;

public:
    static ClientConfig& get();

    ClientConfig(const ClientConfig&) = delete;

    ClientConfig& operator=(const ClientConfig&) = delete;

    const ItemDisplayData& get_item_data(uint8_t item_id) const;

    std::string get_item_name(uint8_t item_id) const;

    std::optional<uint8_t> get_item_id(const std::string& item_name) const;

    std::string get_item_icon_path(uint8_t item_id);

private:
    ClientConfig();

    void loadFromFile(const std::string& filepath);

    void parseItemsTable(const toml::value& items_table);

    ItemDisplayData buildItemDisplayData(const toml::value& item_toml);
};


#endif  // CLIENT_CONFIG_H
