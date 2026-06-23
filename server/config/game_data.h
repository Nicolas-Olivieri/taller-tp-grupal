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

struct ItemData {
    uint16_t price;
    bool is_secret_drop;
};

template <>
struct toml::from<ItemData> {
    static ItemData from_toml(const toml::value& raw) {
        return ItemData{
                toml::find<uint16_t>(raw, "price"),
                toml::find<bool>(raw, "secret_drop"),
        };
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
    uint8_t type_effect;
};

template <>
struct toml::from<WeaponData> {
    static WeaponData from_toml(const toml::value& raw) {
        return WeaponData{
                toml::find<uint8_t>(raw, "range"),
                toml::find<uint8_t>(raw, "mana_cost"),
                toml::find<uint8_t>(raw, "type"),
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

struct BiomeData {
    std::vector<uint8_t> floor_ids;
    std::vector<uint8_t> creatures;
    std::vector<uint8_t> variations;
};

template <>
struct toml::from<BiomeData> {
    static BiomeData from_toml(const toml::value& raw) {
        return BiomeData{
                toml::find<std::vector<uint8_t>>(raw, "floor_ids"),
                toml::find<std::vector<uint8_t>>(raw, "creatures"),
                toml::find<std::vector<uint8_t>>(raw, "variations"),
        };
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
    std::unordered_map<uint8_t, ItemData> items;
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

        const auto& items_table = raw.as_table();

        if (items_table.contains("weapons")) {
            for (const auto& [name, value]: items_table.at("weapons").as_table()) {
                uint8_t id = toml::find<uint8_t>(value, "id");

                WeaponData weapon = toml::get<WeaponData>(value);
                EquipableItemData equipable = toml::get<EquipableItemData>(value);

                data.weapons[id] = std::move(weapon);
                data.equipables[id] = std::move(equipable);
                data.items[id] = toml::get<ItemData>(value);
            }
        }

        add_defensive_equipables(items_table, "armors", data.armors, data);
        add_defensive_equipables(items_table, "helmets", data.helmets, data);
        add_defensive_equipables(items_table, "shields", data.shields, data);

        if (items_table.contains("usables")) {
            for (const auto& [name, value]: items_table.at("usables").as_table()) {
                uint8_t id = toml::find<uint8_t>(value, "id");

                UsableItemData usable = toml::get<UsableItemData>(value);

                data.usables[id] = std::move(usable);
                data.items[id] = toml::get<ItemData>(value);
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

struct FairPlayData {
    uint8_t max_newbie_level;
    uint8_t fair_play_gap;
};

template <>
struct toml::from<FairPlayData> {
    static FairPlayData from_toml(const toml::value& raw) {
        return FairPlayData{toml::find<uint8_t>(raw, "max_newbie_level"),
                            toml::find<uint8_t>(raw, "fair_play_gap")};
    }
};

struct ClanConstantsData {
    uint8_t max_members_per_clan;
    uint8_t min_level_required_to_found_clan;
    float max_distance_to_consider_near_clan_mate;
    float max_attack_buff_factor;
};


template <>
struct toml::from<ClanConstantsData> {
    static ClanConstantsData from_toml(const toml::value& raw) {
        return ClanConstantsData{
                toml::find<uint8_t>(raw, "max_members_per_clan"),
                toml::find<uint8_t>(raw, "min_level_required_to_found_clan"),
                toml::find<float>(raw, "max_distance_to_consider_near_clan_mate"),
                toml::find<float>(raw, "max_attack_buff_factor"),
        };
    }
};

struct WorldConstantsData {
    uint8_t ticks_per_second;
    uint16_t tick_between_saves;
    uint16_t creatures_amount_per_player;
    uint16_t max_player_amount;
    double resurrection_time_factor;
};

template <>
struct toml::from<WorldConstantsData> {
    static WorldConstantsData from_toml(const toml::value& raw) {
        return WorldConstantsData{toml::find<uint8_t>(raw, "ticks_per_second"),
                                  toml::find<uint16_t>(raw, "tick_between_saves"),
                                  toml::find<uint16_t>(raw, "creatures_amount_per_player"),
                                  toml::find<uint16_t>(raw, "max_player_amount"),
                                  toml::find<double>(raw, "resurrection_time_factor")};
    }
};

struct CreatureBehaviorConstantsData {
    uint8_t extra_target_range;
    uint8_t extra_target_range_limit;
    uint8_t attack_cooldowns_to_become_lonely;
};

template <>
struct toml::from<CreatureBehaviorConstantsData> {
    static CreatureBehaviorConstantsData from_toml(const toml::value& raw) {
        return CreatureBehaviorConstantsData{toml::find<uint8_t>(raw, "extra_target_range"),
                                             toml::find<uint8_t>(raw, "extra_target_range_limit"),
                                             toml::find<uint8_t>(raw, "attack_cooldowns_to_become_lonely")};
    }
};

struct BiomesData {
    uint8_t safe_zone_id;
    uint8_t dungeon_id;
    std::unordered_map<uint8_t, uint8_t> floor_to_biome;
    std::unordered_map<uint8_t, BiomeData> biomes;
};

template <>
struct toml::from<BiomesData> {
    static BiomesData from_toml(const toml::value& raw) {
        BiomesData data;
        const auto& biomes_table = raw.as_table();

        if (!biomes_table.contains("biomes"))
            throw std::runtime_error("No se encontró un TOML con la información de biomas");

        for (const auto& [category, value]: biomes_table.at("biomes").as_table()) {
            uint8_t id = toml::find<uint8_t>(value, "id");
            if (category == "safe_zone") {
                data.safe_zone_id = id;
            } else if (category == "dungeon") {
                data.dungeon_id = id;
            }

            auto biome = toml::get<BiomeData>(value);

            data.biomes[id] = biome;
            for (const auto& floor_id: data.biomes[id].floor_ids) data.floor_to_biome[floor_id] = id;
        }

        return data;
    }
};

struct GridConstantsData {
    uint8_t roam_idle_weight;
    uint8_t min_near_factor;
    uint8_t max_near_factor;
};

template <>
struct toml::from<GridConstantsData> {
    static GridConstantsData from_toml(const toml::value& raw) {
        return GridConstantsData{
                toml::find<uint8_t>(raw, "roam_idle_weight"),
                toml::find<uint8_t>(raw, "min_near_factor"),
                toml::find<uint8_t>(raw, "max_near_factor"),
        };
    }
};

struct KillablesConstantsData {
    uint8_t min_level;
    uint8_t max_level;
};

template <>
struct toml::from<KillablesConstantsData> {
    static KillablesConstantsData from_toml(const toml::value& raw) {
        return KillablesConstantsData{toml::find<uint8_t>(raw, "min_level"),
                                      toml::find<uint8_t>(raw, "max_level")};
    }
};

struct CalculatorConstantsData {
    uint8_t base_gold_per_level;
    float pow_gold_per_level;
    float excess_gold_multiplier;
    uint16_t base_xp_limit_per_level;
    float pow_xp_limit_per_level;
    uint8_t added_xp_levels_difference;
    float floor_kill_xp_random_factor;
    float top_kill_xp_random_factor;
    float floor_dodge_random_factor;
    float top_dodge_random_factor;
    float dodge_threshold;
    float floor_gold_drop_random_factor;
    float top_gold_drop_random_factor;
};

template <>
struct toml::from<CalculatorConstantsData> {
    static CalculatorConstantsData from_toml(const toml::value& raw) {
        return CalculatorConstantsData{
                toml::find<uint8_t>(raw, "base_gold_per_level"),
                toml::find<float>(raw, "pow_gold_per_level"),
                toml::find<float>(raw, "excess_gold_multiplier"),
                toml::find<uint16_t>(raw, "base_xp_limit_per_level"),
                toml::find<float>(raw, "pow_xp_limit_per_level"),
                toml::find<uint8_t>(raw, "added_xp_levels_difference"),
                toml::find<float>(raw, "floor_kill_xp_random_factor"),
                toml::find<float>(raw, "top_kill_xp_random_factor"),
                toml::find<float>(raw, "floor_dodge_random_factor"),
                toml::find<float>(raw, "top_dodge_random_factor"),
                toml::find<float>(raw, "dodge_threshold"),
                toml::find<float>(raw, "floor_gold_drop_random_factor"),
                toml::find<float>(raw, "top_gold_drop_random_factor"),
        };
    }
};

struct TraderSettingsData {
    float selling_percentage;
};

template <>
struct toml::from<TraderSettingsData> {
    static TraderSettingsData from_toml(const toml::value& raw) {
        return TraderSettingsData{
                toml::find<float>(raw, "selling_percentage"),
        };
    }
};

struct ClanMessagesData {
    std::string not_in_clan_msg;
    std::string not_a_player_msg;
    std::string is_member_msg;
    std::string is_already_member_msg;
    std::string is_not_in_join_list_msg;
    std::string clan_is_full_msg;
    std::string prefix;
    std::string player_left_clan;
    std::string founder_cannot_leave_clan;
    std::string founder_cannot_kick_himself;
    std::string player_was_banned;
    std::string reject_founder_prefix;
    std::string reject_founder;
    std::string request_rejected;
    std::string player_was_accepted;
    std::string you_were_accepted;
    std::string player_is_banned;
    std::string player_has_clan;
};

template <>
struct toml::from<ClanMessagesData> {
    static ClanMessagesData from_toml(const toml::value& raw) {
        return ClanMessagesData{
                toml::find<std::string>(raw, "not_in_clan_msg"),
                toml::find<std::string>(raw, "not_a_player_msg"),
                toml::find<std::string>(raw, "is_member_msg"),
                toml::find<std::string>(raw, "is_already_member_msg"),
                toml::find<std::string>(raw, "is_not_in_join_list_msg"),
                toml::find<std::string>(raw, "clan_is_full_msg"),
                toml::find<std::string>(raw, "player_prefix"),
                toml::find<std::string>(raw, "player_left_clan"),
                toml::find<std::string>(raw, "founder_cannot_leave_clan"),
                toml::find<std::string>(raw, "founder_cannot_kick_himself"),
                toml::find<std::string>(raw, "player_was_banned"),
                toml::find<std::string>(raw, "reject_founder_prefix"),
                toml::find<std::string>(raw, "reject_founder"),
                toml::find<std::string>(raw, "request_rejected"),
                toml::find<std::string>(raw, "player_was_accepted"),
                toml::find<std::string>(raw, "you_were_accepted"),
                toml::find<std::string>(raw, "player_is_banned"),
                toml::find<std::string>(raw, "player_has_clan"),
        };
    }
};


#endif  // GAME_DATA_H
