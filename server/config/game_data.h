#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <toml.hpp>

/* Paths hacia el resto de tomls con datos del juego */

struct PathsData {
    std::unordered_map<std::string, std::string> paths;
};

template <>
struct toml::from<PathsData> {
    static PathsData from_toml(const toml::value& raw) {
        PathsData data;

        if (!raw.contains("server"))
            throw std::runtime_error("No se encontró el archivo TOML con los paths hacia el resto de TOMLS");

        const auto server_table = toml::find(raw, "server").as_table();
        for (const auto& [category, content]: server_table)
            data.paths[category] = toml::find<std::string>(content, "path");

        return data;
    }
};

/* Estructuras contenedores de los datos de configuración */

struct ArchetypeData {
    float health_factor;
    float mana_factor;
    float meditation_factor;
    uint8_t agility;
    float constitution_multiplier;
    float intelligence_multiplier;
    float strength_multiplier;
};

template <>
struct toml::from<ArchetypeData> {
    static ArchetypeData from_toml(const toml::value& raw) {
        return ArchetypeData{
                toml::find<float>(raw, "health_factor"),
                toml::find<float>(raw, "mana_factor"),
                toml::find<float>(raw, "meditation_factor"),
                toml::find<uint8_t>(raw, "agility"),
                toml::find<float>(raw, "constitution_multiplier"),
                toml::find<float>(raw, "intelligence_multiplier"),
                toml::find<float>(raw, "strength_multiplier"),
        };
    }
};

struct RaceData {
    float health_factor;
    float mana_factor;
    uint8_t recovery_factor;
    uint8_t agility;
    uint8_t constitution;
    uint8_t intelligence;
    uint8_t strength;
};

template <>
struct toml::from<RaceData> {
    static RaceData from_toml(const toml::value& raw) {
        return RaceData{
                toml::find<float>(raw, "health_factor"),     toml::find<float>(raw, "mana_factor"),
                toml::find<uint8_t>(raw, "recovery_factor"), toml::find<uint8_t>(raw, "agility"),
                toml::find<uint8_t>(raw, "constitution"),    toml::find<uint8_t>(raw, "intelligence"),
                toml::find<uint8_t>(raw, "strength"),
        };
    }
};

struct VariationData {
    std::vector<uint8_t> compatible_races;
    float factor;
    uint8_t agility;
    float multiplier;
    float max_level_multiplier;
    std::vector<uint8_t> equipment;
};

template <>
struct toml::from<VariationData> {
    static VariationData from_toml(const toml::value& raw) {
        return VariationData{toml::find<std::vector<uint8_t>>(raw, "compatible_races"),
                             toml::find<float>(raw, "factor"),
                             toml::find<uint8_t>(raw, "agility"),
                             toml::find<float>(raw, "multiplier"),
                             toml::find<float>(raw, "max_level_multiplier"),
                             toml::find<std::vector<uint8_t>>(raw, "equipment")};
    }
};

struct EquipableItemData {
    uint8_t min;
    uint8_t max;
};

template <>
struct toml::from<EquipableItemData> {
    static EquipableItemData from_toml(const toml::value& raw) {
        return EquipableItemData{
                toml::find<uint8_t>(raw, "min_value"),
                toml::find<uint8_t>(raw, "max_value"),
        };
    }
};

struct UsableItemData {
    uint8_t type_effect;
    uint8_t effect_amount;
};

template <>
struct toml::from<UsableItemData> {
    static UsableItemData from_toml(const toml::value& raw) {
        return UsableItemData{
                toml::find<uint8_t>(raw, "type"),
                toml::find<uint8_t>(raw, "effect"),
        };
    }
};

struct WeaponData {
    uint8_t range;
    uint8_t mana_cost;
};

template <>
struct toml::from<WeaponData> {
    static WeaponData from_toml(const toml::value& raw) {
        return WeaponData{
                toml::find<uint8_t>(raw, "range"),
                toml::find<uint8_t>(raw, "mana_cost"),
        };
    }
};

struct DropProbabilitiesData {
    float nothing;
    float gold;
    float usable;
    float equipable;
};

template <>
struct toml::from<DropProbabilitiesData> {
    static DropProbabilitiesData from_toml(const toml::value& raw) {
        return DropProbabilitiesData{toml::find<float>(raw, "nothing"), toml::find<float>(raw, "gold"),
                                     toml::find<float>(raw, "usable"), toml::find<float>(raw, "equipable")};
    }
};

struct CooldownData {
    uint8_t attack;
    uint8_t move;
};

template <>
struct toml::from<CooldownData> {
    static CooldownData from_toml(const toml::value& raw) {
        return CooldownData{toml::find<uint8_t>(raw, "attack"), toml::find<uint8_t>(raw, "move")};
    }
};

struct TraderSetData {
    std::vector<std::vector<uint8_t>> items;
};

template <>
struct toml::from<TraderSetData> {
    static TraderSetData from_toml(const toml::value& raw) {
        TraderSetData data;
        const auto& table = raw.as_table();

        data.items.resize(table.size());
        for (const auto& [key, value]: table) {
            data.items[std::stoi(key)] = toml::get<std::vector<uint8_t>>(value);
        }

        return data;
    }
};

/* Estructuras que encapsulan cada archivo de tomls */

struct PlayerStatsData {
    std::unordered_map<uint8_t, ArchetypeData> archetypes;
    std::unordered_map<uint8_t, RaceData> races;
};

template <>
struct toml::from<PlayerStatsData> {
    static PlayerStatsData from_toml(const toml::value& raw) {
        PlayerStatsData data;
        const auto& players_stats_table = raw.as_table();

        if (!players_stats_table.contains("archetypes"))
            throw std::runtime_error("No se encontró un TOML con la información arquetipos de jugador");

        for (const auto& [category, value]: players_stats_table.at("archetypes").as_table()) {
            auto archetype = toml::get<ArchetypeData>(value);
            data.archetypes[toml::find<uint8_t>(value, "id")] = std::move(archetype);
        }

        if (!players_stats_table.contains("races"))
            throw std::runtime_error("No se encontró un TOML con la información razas de jugador");

        for (const auto& [category, value]: players_stats_table.at("races").as_table()) {
            auto race = toml::get<RaceData>(value);
            data.races[toml::find<uint8_t>(value, "id")] = std::move(race);
        }

        return data;
    }
};

struct CreatureStatsData {
    std::unordered_map<uint8_t, RaceData> creatures;
    std::unordered_map<uint8_t, uint8_t> base_creature_level;
    std::unordered_map<uint8_t, VariationData> variations;
};

template <>
struct toml::from<CreatureStatsData> {
    static CreatureStatsData from_toml(const toml::value& raw) {
        CreatureStatsData data;
        const auto& creature_stats_table = raw.as_table();

        if (!creature_stats_table.contains("races"))
            throw std::runtime_error("No se encontró un TOML con la información razas de criaturas");

        for (const auto& [name, value]: creature_stats_table.at("races").as_table()) {
            uint8_t id = toml::find<uint8_t>(value, "id");
            auto race = toml::get<RaceData>(value);
            data.creatures[id] = std::move(race);
            data.base_creature_level[id] = toml::find<uint8_t>(value, "base_level");
        }

        if (!creature_stats_table.contains("variations"))
            throw std::runtime_error("No se encontró un TOML con la información variaciones de criaturas");

        for (const auto& [name, value]: creature_stats_table.at("variations").as_table()) {
            auto variation = toml::get<VariationData>(value);
            data.variations[toml::find<uint8_t>(value, "id")] = std::move(variation);
        }

        return data;
    }
};

struct ItemsData {
    uint8_t min_equipable_id;
    uint8_t max_equipable_id;
    uint8_t min_usable_id;
    uint8_t max_usable_id;
    std::unordered_map<uint8_t, uint16_t> prices;
    std::unordered_map<uint8_t, EquipableItemData> equipables;
    std::unordered_map<uint8_t, UsableItemData> usables;
    std::unordered_map<uint8_t, WeaponData> weapons;
    std::unordered_set<uint8_t> armors;
    std::unordered_set<uint8_t> helmets;
    std::unordered_set<uint8_t> shields;
};

template <>
struct toml::from<ItemsData> {
    static ItemsData from_toml(const toml::value& raw) {
        ItemsData data;
        data.min_equipable_id = UINT8_MAX;
        data.max_equipable_id = 0;
        data.min_usable_id = UINT8_MAX;
        data.max_usable_id = 0;

        const auto& items_table = raw.as_table();

        if (items_table.contains("weapons")) {
            for (const auto& [name, value]: items_table.at("weapons").as_table()) {
                uint8_t id = toml::find<uint8_t>(value, "id");
                update_min_max_id(data.min_equipable_id, data.max_equipable_id, id);

                WeaponData weapon = toml::get<WeaponData>(value);
                EquipableItemData equipable = toml::get<EquipableItemData>(value);

                data.weapons[id] = std::move(weapon);
                data.equipables[id] = std::move(equipable);
                data.prices[id] = toml::find<uint16_t>(value, "price");
            }
        }

        add_defensive_equipables(items_table, "armors", data.armors, data);
        add_defensive_equipables(items_table, "helmets", data.helmets, data);
        add_defensive_equipables(items_table, "shields", data.shields, data);

        if (items_table.contains("usables")) {
            for (const auto& [name, value]: items_table.at("usables").as_table()) {
                uint8_t id = toml::find<uint8_t>(value, "id");
                update_min_max_id(data.min_usable_id, data.max_usable_id, id);

                UsableItemData usable = toml::get<UsableItemData>(value);

                data.usables[id] = std::move(usable);
                data.prices[id] = toml::find<uint16_t>(value, "price");
            }
        }

        return data;
    }

    static void update_min_max_id(uint8_t& current_min, uint8_t& current_max, uint8_t id);

    static void add_defensive_equipables(
            const toml::type_config::table_type<toml::type_config::string_type, toml::value>& table,
            const std::string& category, std::unordered_set<uint8_t>& category_set, ItemsData& data);
};

struct CooldownsData {
    CooldownData players;
    CooldownData creatures;
};

template <>
struct toml::from<CooldownsData> {
    static CooldownsData from_toml(const toml::value& raw) {
        CooldownsData data;
        const auto& cooldowns_table = raw.as_table();

        if (!cooldowns_table.contains("cooldowns"))
            throw std::runtime_error(
                    "No se encontró un TOML con la información de cooldowns para players y creatures");

        const auto& cooldown_node = cooldowns_table.at("cooldowns");

        data.players = toml::find<CooldownData>(cooldown_node, "player");
        data.creatures = toml::find<CooldownData>(cooldown_node, "creature");

        return data;
    }
};

struct TradersData {
    TraderSetData priests;
    TraderSetData merchants;
};

template <>
struct toml::from<TradersData> {
    static TradersData from_toml(const toml::value& raw) {
        TradersData data;
        const auto& traders_table = raw.as_table();

        if (!traders_table.contains("traders"))
            throw std::runtime_error("No se encontró un TOML con la información de comerciantes");

        const auto& traders_node = traders_table.at("traders");

        data.priests = toml::find<TraderSetData>(traders_node, "priests");
        data.merchants = toml::find<TraderSetData>(traders_node, "merchants");

        return data;
    }
};

#endif  // GAME_DATA_H
