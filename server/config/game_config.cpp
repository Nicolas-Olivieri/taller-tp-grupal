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

uint16_t GameConfig::get_item_price(uint8_t item_id) const { return items.prices.at(item_id); }

bool GameConfig::usables_contains(uint8_t id) const { return items.usables.contains(id); }

bool GameConfig::weapons_contains(uint8_t id) const { return items.weapons.contains(id); }

bool GameConfig::helmets_contains(uint8_t id) const { return items.helmets.contains(id); }

bool GameConfig::armors_contains(uint8_t id) const { return items.armors.contains(id); }

bool GameConfig::shields_contains(uint8_t id) const { return items.shields.contains(id); }

uint8_t GameConfig::get_min_usable_id() const { return items.min_usable_id; }

uint8_t GameConfig::get_max_usable_id() const { return items.max_usable_id; }

uint8_t GameConfig::get_min_equipable_id() const { return items.min_equipable_id; }

uint8_t GameConfig::get_max_equipable_id() const { return items.max_usable_id; }

const std::vector<uint8_t>& GameConfig::get_priest_items(int id) const { return traders.priests.items[id]; }

int GameConfig::get_priest_max_id() const { return traders.priests.items.size() - 1; }

const std::vector<uint8_t>& GameConfig::get_merchant_items(int id) const {
    return traders.merchants.items[id];
}

int GameConfig::get_merchant_max_id() const { return traders.merchants.items.size() - 1; }
