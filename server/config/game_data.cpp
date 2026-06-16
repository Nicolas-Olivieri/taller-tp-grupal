#include "game_data.h"

void toml::from<ItemsData>::add_defensive_equipables(
        const toml::type_config::table_type<toml::type_config::string_type, toml::value>& table,
        const std::string& category, std::unordered_set<uint8_t>& category_set, ItemsData& data) {
    if (table.contains(category)) {
        for (const auto& [name, value]: table.at(category).as_table()) {
            uint8_t id = toml::find<uint8_t>(value, "id");

            EquipableItemData equipable = toml::get<EquipableItemData>(value);

            category_set.insert(id);
            data.equipables[id] = std::move(equipable);
            data.items[id] = toml::get<ItemData>(value);
        }
    }
}
