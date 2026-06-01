#include "game_config.h"

#include <string>

#include <toml.hpp>
#define TOML_PATH "/paths.toml"

GameConfig::GameConfig() {
    auto root = toml::parse(DATA_PATH TOML_PATH);

    const auto paths = toml::find(root, "server");
    const auto stats_path = toml::find(paths, "stats");
    const auto stats_file = toml::find<std::string>(stats_path, "path");
    auto stats_root = toml::parse(DATA_PATH + stats_file);

    const auto archetypes_table = toml::find(stats_root, "archetypes");
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

    const auto races_table = toml::find(stats_root, "races");
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

    CooldownData cooldown_data{};
    const auto cooldowns_path = toml::find(paths, "cooldowns");
    const auto cooldowns_file = toml::find<std::string>(cooldowns_path, "path");
    const auto cooldowns_root = toml::parse(DATA_PATH + cooldowns_file);
    const auto cooldowns_table = toml::find(cooldowns_root, "cooldowns");
    cooldown_data.attack = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "attack"));
    cooldown_data.move = static_cast<uint8_t>(toml::find<int>(cooldowns_table, "move"));
    cooldowns = cooldown_data;
}


GameConfig& GameConfig::get() {
    static GameConfig instance;
    return instance;
}

const ArchetypeData& GameConfig::get_archetype(const uint8_t id) const { return this->archetypes.at(id); }

const RaceData& GameConfig::get_race(const uint8_t id) const { return this->races.at(id); }

const CooldownData& GameConfig::get_cooldown() const { return this->cooldowns; }
