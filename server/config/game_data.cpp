#include "game_data.h"

void toml::from<ItemsData>::update_min_max_id(uint8_t& current_min, uint8_t& current_max, uint8_t id) {
    if (id < current_min)
        current_min = id;

    if (id > current_max)
        current_max = id;
}

void toml::from<ItemsData>::add_defensive_equipables(
        const toml::type_config::table_type<toml::type_config::string_type, toml::value>& table,
        const std::string& category, std::unordered_set<uint8_t>& category_set, ItemsData& data) {
    if (table.contains(category)) {
        for (const auto& [name, value]: table.at(category).as_table()) {
            uint8_t id = toml::find<uint8_t>(value, "id");
            update_min_max_id(data.min_equipable_id, data.max_equipable_id, id);

            EquipableItemData equipable = toml::get<EquipableItemData>(value);

            category_set.insert(id);
            data.equipables[id] = std::move(equipable);
            data.prices[id] = toml::find<uint16_t>(value, "price");
        }
    }
}
