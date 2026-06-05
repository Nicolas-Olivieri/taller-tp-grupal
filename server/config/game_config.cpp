#include "game_config.h"

#include <string>

#include <toml.hpp>
#define TOML_PATH "/paths.toml"

GameConfig::GameConfig() {
    auto root = toml::parse(DATA_PATH TOML_PATH);

    const auto paths = toml::find(root, "server");
    const auto player_stats_path = toml::find(paths, "players");
    const auto player_stats_file = toml::find<std::string>(player_stats_path, "path");
    auto player_stats_root = toml::parse(DATA_PATH + player_stats_file);

    const auto archetypes_table = toml::find(player_stats_root, "archetypes");
    for (const auto& [name, value]: archetypes_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        ArchetypeData archetype{};
        archetype.health_factor = static_cast<float>(toml::find<double>(value, "health_factor"));
        archetype.mana_factor = static_cast<float>(toml::find<double>(value, "mana_factor"));
        archetype.meditation_factor = static_cast<float>(toml::find<double>(value, "meditation_factor"));
        archetype.agility = static_cast<uint8_t>(toml::find<int>(value, "agility"));
        archetype.constitution_multiplier =
                static_cast<float>(toml::find<double>(value, "constitution_multiplier"));
        archetype.intelligence_multiplier =
                static_cast<float>(toml::find<double>(value, "intelligence_multiplier"));
        archetype.strength_multiplier = static_cast<float>(toml::find<double>(value, "strength_multiplier"));

        archetypes[id] = archetype;
    }

    const auto races_table = toml::find(player_stats_root, "races");
    for (const auto& [name, value]: races_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        RaceData race{};
        race.health_factor = static_cast<float>(toml::find<double>(value, "health_factor"));
        race.mana_factor = static_cast<float>(toml::find<double>(value, "mana_factor"));
        race.recovery_factor = static_cast<uint8_t>(toml::find<int>(value, "recovery_factor"));
        race.agility = static_cast<uint8_t>(toml::find<int>(value, "agility"));
        race.constitution = static_cast<uint8_t>(toml::find<int>(value, "constitution"));
        race.intelligence = static_cast<uint8_t>(toml::find<int>(value, "intelligence"));
        race.strength = static_cast<uint8_t>(toml::find<int>(value, "strength"));

        races[id] = race;
    }

    const auto creature_stats_path = toml::find(paths, "creatures");
    const auto creature_stats_file = toml::find<std::string>(creature_stats_path, "path");
    auto creature_stats_root = toml::parse(DATA_PATH + creature_stats_file);

    const auto creatures_table = toml::find(creature_stats_root, "races");
    for (const auto& [name, value]: creatures_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        RaceData creature{};
        creature.health_factor = static_cast<float>(toml::find<double>(value, "health_factor"));
        creature.mana_factor = static_cast<float>(toml::find<double>(value, "mana_factor"));
        creature.recovery_factor = static_cast<uint8_t>(toml::find<int>(value, "recovery_factor"));
        creature.agility = static_cast<uint8_t>(toml::find<int>(value, "agility"));
        creature.constitution = static_cast<uint8_t>(toml::find<int>(value, "constitution"));
        creature.intelligence = static_cast<uint8_t>(toml::find<int>(value, "intelligence"));
        creature.strength = static_cast<uint8_t>(toml::find<int>(value, "strength"));

        creatures[id] = creature;
    }

    const auto variations_table = toml::find(creature_stats_root, "variations");
    for (const auto& [name, value]: variations_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        VariationData variation{};
        const auto races_vector = toml::find<std::vector<int>>(value, "compatible_races");
        variation.compatible_races = std::vector<uint8_t>(races_vector.begin(), races_vector.end());
        variation.factor = static_cast<float>(toml::find<double>(value, "factor"));
        variation.agility = static_cast<uint8_t>(toml::find<int>(value, "agility"));
        variation.multiplier = static_cast<float>(toml::find<double>(value, "multiplier"));

        variations[id] = variation;
    }

    const auto cooldowns_path = toml::find(paths, "cooldowns");
    const auto cooldowns_file = toml::find<std::string>(cooldowns_path, "path");
    const auto cooldowns_root = toml::parse(DATA_PATH + cooldowns_file);
    const auto cooldowns_table = toml::find(cooldowns_root, "cooldowns");

    player_cooldowns.attack = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "player", "attack"));
    player_cooldowns.move = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "player", "move"));

    creature_cooldowns.attack = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "creature", "attack"));
    creature_cooldowns.move = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "creature", "move"));

    const auto items_path = toml::find(paths, "items");
    const auto items_file = toml::find<std::string>(items_path, "path");
    const auto items_root = toml::parse(DATA_PATH + items_file);
    const auto items_potions_table = toml::find(items_root, "potions");
    for (const auto& [name, value]: items_potions_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        UsableItemData itemData{};
        itemData.type_effect = static_cast<uint8_t>(toml::find<int>(value, "type"));
        itemData.effect_amount = static_cast<uint16_t>(toml::find<int>(value, "effect"));

        usable_items[id] = itemData;
    }

    const auto items_weapons_table = toml::find(items_root, "weapons");
    for (const auto& [name, value]: items_weapons_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        WeaponItemData itemData{};
        itemData.itemData.min = static_cast<uint16_t>(toml::find<int>(value, "min_damage"));
        itemData.itemData.max = static_cast<uint16_t>(toml::find<int>(value, "max_damage"));
        itemData.range = static_cast<uint8_t>(toml::find<int>(value, "range"));
        itemData.mana_cost = static_cast<uint16_t>(toml::find<int>(value, "mana_cost"));

        weapons_items[id] = itemData;
    }

    const auto items_helmets_table = toml::find(items_root, "helmets");
    for (const auto& [name, value]: items_helmets_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        EquipableItemData itemData{};
        itemData.min = static_cast<uint16_t>(toml::find<int>(value, "min_defense"));
        itemData.max = static_cast<uint16_t>(toml::find<int>(value, "max_defense"));

        helmets_items[id] = itemData;
    }

    const auto items_armors_table = toml::find(items_root, "armors");
    for (const auto& [name, value]: items_armors_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        EquipableItemData itemData{};
        itemData.min = static_cast<uint16_t>(toml::find<int>(value, "min_defense"));
        itemData.max = static_cast<uint16_t>(toml::find<int>(value, "max_defense"));

        armors_items[id] = itemData;
    }

    const auto items_shields_table = toml::find(items_root, "shields");
    for (const auto& [name, value]: items_shields_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        EquipableItemData itemData{};
        itemData.min = static_cast<uint16_t>(toml::find<int>(value, "min_defense"));
        itemData.max = static_cast<uint16_t>(toml::find<int>(value, "max_defense"));

        shields_items[id] = itemData;
    }
}


GameConfig& GameConfig::get() {
    static GameConfig instance;
    return instance;
}

const ArchetypeData& GameConfig::get_archetype(const uint8_t id) const { return this->archetypes.at(id); }

const RaceData& GameConfig::get_race(const uint8_t id) const { return this->races.at(id); }

const RaceData& GameConfig::get_creature(uint8_t id) const { return this->creatures.at(id); }

const VariationData& GameConfig::get_variation(uint8_t id) const { return this->variations.at(id); }

const CooldownData& GameConfig::get_player_cooldown() const { return this->player_cooldowns; }

const CooldownData& GameConfig::get_creature_cooldown() const { return this->creature_cooldowns; }

const std::unordered_map<uint8_t, UsableItemData>& GameConfig::get_usables() const { return usable_items; }

const std::unordered_map<uint8_t, WeaponItemData>& GameConfig::get_weapons() const { return weapons_items; }

const std::unordered_map<uint8_t, EquipableItemData>& GameConfig::get_helmets() const {
    return helmets_items;
}

const std::unordered_map<uint8_t, EquipableItemData>& GameConfig::get_armors() const { return armors_items; }

const std::unordered_map<uint8_t, EquipableItemData>& GameConfig::get_shields() const {
    return shields_items;
}

uint16_t GameConfig::get_item_price(const uint8_t item_id) const {
    if (not item_prices.contains(item_id)) {
        throw std::out_of_range(
                "GameConfig recibió un ítem que no se encuentra en la configuración del servidor: " +
                std::to_string(item_id));
    }

    return item_prices.at(item_id);
}
