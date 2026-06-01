#include "statsconfig.h"

#include <toml.hpp>

StatsConfig::StatsConfig() {
    auto root = toml::parse(DATA_PATH "/stats.toml");

    const auto archetypes_table = toml::find(root, "archetypes");
    for (const auto& [name, value]: archetypes_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        ArchetypeData archetype;
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

    const auto races_table = toml::find(root, "races");
    for (const auto& [name, value]: races_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));

        RaceData race;
        race.health_factor = static_cast<float>(toml::find<double>(value, "health_factor"));
        race.mana_factor = static_cast<float>(toml::find<double>(value, "mana_factor"));
        race.recovery_factor = static_cast<uint8_t>(toml::find<int>(value, "recovery_factor"));
        race.agility = static_cast<uint8_t>(toml::find<int>(value, "agility"));
        race.constitution = static_cast<uint8_t>(toml::find<int>(value, "constitution"));
        race.intelligence = static_cast<uint8_t>(toml::find<int>(value, "intelligence"));
        race.strength = static_cast<uint8_t>(toml::find<int>(value, "strength"));

        races[id] = race;
    }
}

StatsConfig& StatsConfig::get() {
    static StatsConfig instance;
    return instance;
}

const ArchetypeData& StatsConfig::get_archetype(uint8_t id) const { return this->archetypes.at(id); }

const RaceData& StatsConfig::get_race(uint8_t id) const { return this->races.at(id); }
