#include "game_config.h"

#include <string>

#include <toml.hpp>
#define TOML_PATH "/paths.toml"

GameConfig::GameConfig() {
    auto root = toml::parse(CONFIG_PATH TOML_PATH);

    const auto paths = toml::find(root, "server");
    const auto player_stats_path = toml::find(paths, "players");
    const auto player_stats_file = toml::find<std::string>(player_stats_path, "path");
    auto player_stats_root = toml::parse(CONFIG_PATH + player_stats_file);

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
    auto creature_stats_root = toml::parse(CONFIG_PATH + creature_stats_file);

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
    const auto cooldowns_root = toml::parse(CONFIG_PATH + cooldowns_file);
    const auto cooldowns_table = toml::find(cooldowns_root, "cooldowns");

    player_cooldowns.attack = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "player", "attack"));
    player_cooldowns.move = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "player", "move"));

    creature_cooldowns.attack = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "creature", "attack"));
    creature_cooldowns.move = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "creature", "move"));

    const auto items_path = toml::find(paths, "items");
    const auto items_file = toml::find<std::string>(items_path, "path");
    const auto items_root = toml::parse(CONFIG_PATH + items_file);
    const auto items_potions_table = toml::find(items_root, "potions");
    for (const auto& [name, value]: items_potions_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        UsableItemData data{};
        data.type_effect = static_cast<uint8_t>(toml::find<int>(value, "type"));
        data.effect_amount = static_cast<uint16_t>(toml::find<int>(value, "effect"));

        usables[id] = data;
    }

    const auto items_weapons_table = toml::find(items_root, "weapons");
    for (const auto& [name, value]: items_weapons_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        EquipableItemData equipable_data{};
        WeaponData weapon_data{};
        equipable_data.min = static_cast<uint16_t>(toml::find<int>(value, "min_damage"));
        equipable_data.max = static_cast<uint16_t>(toml::find<int>(value, "max_damage"));
        weapon_data.range = static_cast<uint8_t>(toml::find<int>(value, "range"));
        weapon_data.mana_cost = static_cast<uint16_t>(toml::find<int>(value, "mana_cost"));

        equipables[id] = equipable_data;
        weapons[id] = weapon_data;
    }

    const auto items_helmets_table = toml::find(items_root, "helmets");
    for (const auto& [name, value]: items_helmets_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        EquipableItemData equipable_data{};
        equipable_data.min = static_cast<uint16_t>(toml::find<int>(value, "min_defense"));
        equipable_data.max = static_cast<uint16_t>(toml::find<int>(value, "max_defense"));

        equipables[id] = equipable_data;
        helmets.insert(id);
    }

    const auto items_armors_table = toml::find(items_root, "armors");
    for (const auto& [name, value]: items_armors_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        EquipableItemData equipable_data{};
        equipable_data.min = static_cast<uint16_t>(toml::find<int>(value, "min_defense"));
        equipable_data.max = static_cast<uint16_t>(toml::find<int>(value, "max_defense"));

        equipables[id] = equipable_data;
        armors.insert(id);
    }

    const auto items_shields_table = toml::find(items_root, "shields");
    for (const auto& [name, value]: items_shields_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        item_prices[id] = static_cast<uint16_t>(toml::find<int>(value, "price"));

        EquipableItemData equipable_data{};
        equipable_data.min = static_cast<uint16_t>(toml::find<int>(value, "min_defense"));
        equipable_data.max = static_cast<uint16_t>(toml::find<int>(value, "max_defense"));

        equipables[id] = equipable_data;
        shields.insert(id);
    }

    const auto traders_path = toml::find(paths, "traders");
    const auto traders_file = toml::find<std::string>(traders_path, "path");
    auto traders_root = toml::parse(DATA_PATH + traders_file);

    const auto priests_table = toml::find(traders_root, "priests");
    for (const auto& [name, value]: priests_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(std::stoi(name));
        priest_items[id] = toml::get<std::vector<uint8_t>>(value);
    }

    const auto merchants_table = toml::find(traders_root, "merchants");
    for (const auto& [name, value]: merchants_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(std::stoi(name));
        merchant_items[id] = toml::get<std::vector<uint8_t>>(value);
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

const EquipableItemData& GameConfig::get_equipable(uint8_t id) const { return this->equipables.at(id); }

const UsableItemData& GameConfig::get_usable(uint8_t id) const { return this->usables.at(id); }

const WeaponData& GameConfig::get_weapon(uint8_t id) const { return this->weapons.at(id); }

bool GameConfig::usables_contains(uint8_t id) const { return this->usables.contains(id); }

bool GameConfig::weapons_contains(uint8_t id) const { return this->weapons.contains(id); }

bool GameConfig::helmets_contains(uint8_t id) const { return this->helmets.contains(id); }

bool GameConfig::armors_contains(uint8_t id) const { return this->armors.contains(id); }

bool GameConfig::shields_contains(uint8_t id) const { return this->shields.contains(id); }

uint16_t GameConfig::get_item_price(const uint8_t item_id) const {
    if (not item_prices.contains(item_id)) {
        throw std::out_of_range(
                "GameConfig recibió un ítem que no se encuentra en la configuración del servidor: " +
                std::to_string(item_id));
    }

    return item_prices.at(item_id);
}

const std::vector<uint8_t>& GameConfig::get_priest_items(int id) const { return priest_items.at(id); }

int GameConfig::get_priest_max_id() const { return priest_items.size() - 1; }

const std::vector<uint8_t>& GameConfig::get_merchant_items(int id) const { return merchant_items.at(id); }

int GameConfig::get_merchant_max_id() const { return merchant_items.size() - 1; }
