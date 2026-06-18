#include "game_config.h"

#define TOML_PATH "/paths.toml"

GameConfig::GameConfig() {
    const auto paths_root = toml::parse(CONFIG_PATH TOML_PATH);
    PathsData paths_data = toml::get<PathsData>(paths_root);

    player_stats = retrieve_config_data<PlayerStatsData>(paths_data, "players");
    creature_stats = retrieve_config_data<CreatureStatsData>(paths_data, "creatures");
    cooldowns = retrieve_config_data<CooldownsData>(paths_data, "cooldowns");
    items = retrieve_config_data<ItemsData>(paths_data, "items");
    traders = retrieve_config_data<TradersData>(paths_data, "traders");
    drop_probabilities = retrieve_config_data<DropProbabilitiesData>(paths_data, "drops", "probabilities");
    fair_play_levels = retrieve_config_data<FairPlayData>(paths_data, "fair_play", "fair_play");
    clan_constants_data = retrieve_config_data<ClanConstantsData>(paths_data, "clans", "clans");
    biomes_data = retrieve_config_data<BiomesData>(paths_data, "biomes");
    world_constants_data = retrieve_config_data<WorldConstantsData>(paths_data, "world", "world");
    creature_constants_data = retrieve_config_data<CreatureBehaviorConstantsData>(
            paths_data, "creature_behavior", "creature_behavior");
    grid_constants_data = retrieve_config_data<GridConstantsData>(paths_data, "world", "grid");
}

GameConfig& GameConfig::get() {
    static GameConfig instance;
    return instance;
}

const ArchetypeData& GameConfig::get_archetype(uint8_t id) const { return player_stats.archetypes.at(id); }

const RaceData& GameConfig::get_race(uint8_t id) const { return player_stats.races.at(id); }

const RaceData& GameConfig::get_creature(uint8_t id) const { return creature_stats.creatures.at(id); }

uint8_t GameConfig::get_creature_base_level(uint8_t id) const {
    return creature_stats.base_creature_level.at(id);
}

const VariationData& GameConfig::get_variation(uint8_t id) const { return creature_stats.variations.at(id); }

const CooldownData& GameConfig::get_player_cooldown() const { return cooldowns.players; }

const CooldownData& GameConfig::get_creature_cooldown() const { return cooldowns.creatures; }

const EquipableItemData& GameConfig::get_equipable(uint8_t id) const { return items.equipables.at(id); }

const UsableItemData& GameConfig::get_usable(uint8_t id) const { return items.usables.at(id); }

const WeaponData& GameConfig::get_weapon(uint8_t id) const { return items.weapons.at(id); }

const DropProbabilitiesData& GameConfig::get_drop_probabilities() const { return drop_probabilities; }

uint16_t GameConfig::get_item_price(uint8_t item_id) const { return items.items.at(item_id).price; }

bool GameConfig::is_secret_item(uint8_t item_id) const { return items.items.at(item_id).is_secret_drop; }

bool GameConfig::usables_contains(uint8_t id) const { return items.usables.contains(id); }

bool GameConfig::weapons_contains(uint8_t id) const { return items.weapons.contains(id); }

bool GameConfig::helmets_contains(uint8_t id) const { return items.helmets.contains(id); }

bool GameConfig::armors_contains(uint8_t id) const { return items.armors.contains(id); }

bool GameConfig::shields_contains(uint8_t id) const { return items.shields.contains(id); }

std::vector<uint8_t> GameConfig::get_regular_usables_ids() const {
    std::vector<uint8_t> usables_ids;
    usables_ids.reserve(items.usables.size());

    for (const auto& [id, value]: items.usables) {
        if (!items.items.at(id).is_secret_drop)
            usables_ids.push_back(id);
    }

    return usables_ids;
}

std::vector<uint8_t> GameConfig::get_regular_equipables_ids() const {
    std::vector<uint8_t> equipables_ids;
    equipables_ids.reserve(items.equipables.size());

    for (const auto& [id, value]: items.equipables) {
        if (!items.items.at(id).is_secret_drop)
            equipables_ids.push_back(id);
    }

    return equipables_ids;
}

std::vector<uint8_t> GameConfig::get_secret_equipables_ids() const {
    std::vector<uint8_t> equipables_ids;
    equipables_ids.reserve(items.equipables.size());

    for (const auto& [id, value]: items.equipables) {
        if (items.items.at(id).is_secret_drop)
            equipables_ids.push_back(id);
    }

    return equipables_ids;
}

const std::vector<uint8_t>& GameConfig::get_priest_items(int id) const { return traders.priests.items[id]; }

int GameConfig::get_priest_max_id() const { return traders.priests.items.size() - 1; }

const std::vector<uint8_t>& GameConfig::get_merchant_items(int id) const {
    return traders.merchants.items[id];
}

int GameConfig::get_merchant_max_id() const { return traders.merchants.items.size() - 1; }

const FairPlayData& GameConfig::get_fair_play() const { return fair_play_levels; }

const ClanConstantsData& GameConfig::get_clan_constants() const { return clan_constants_data; }

const WorldConstantsData& GameConfig::get_world_constants() const { return world_constants_data; }

const CreatureBehaviorConstantsData& GameConfig::get_creature_behavior_constants() const {
    return creature_constants_data;
}
const ClanConstantsData& GameConfig::get_clan_constats() const { return clan_constants_data; }

const BiomeData& GameConfig::get_biome_from_floor(uint8_t id) const {
    assert(biomes_data.floor_to_biome.contains(id));
    uint8_t biome_id = biomes_data.floor_to_biome.at(id);

    assert(biomes_data.biomes.contains(biome_id));
    return biomes_data.biomes.at(biome_id);
}

bool GameConfig::has_biome_associated(uint8_t id) const { return biomes_data.floor_to_biome.contains(id); }

const GridConstantsData& GameConfig::get_grid_constants() const { return grid_constants_data; }

bool GameConfig::is_safe_zone_floor(uint8_t floor_id) {
    return has_biome_associated(floor_id) && floor_id == biomes_data.safe_zone_id;
}

bool GameConfig::is_dungeon_floor(uint8_t floor_id) {
    return has_biome_associated(floor_id) && floor_id == biomes_data.dungeon_id;
}
