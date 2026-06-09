#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <string>
#include <vector>

#include "game_data.h"

class GameConfig {
private:
    PlayerStatsData player_stats;
    CreatureStatsData creature_stats;
    ItemsData items;
    CooldownsData cooldowns;
    TradersData traders;
    DropProbabilitiesData drop_probabilities;

    GameConfig();

    template <typename T>
    T retrieve_config_data(const PathsData& data, const std::string& path_id) {
        if (!data.paths.contains(path_id))
            throw std::runtime_error("No se encontró el path al archivo TOML: " + path_id);

        const auto root = toml::parse(CONFIG_PATH + data.paths.at(path_id));

        return toml::get<T>(root);
    }

    template <typename T>
    T retrieve_config_data(const PathsData& data, const std::string& path_id,
                           const std::string& sub_category) {
        if (!data.paths.contains(path_id))
            throw std::runtime_error("No se encontró el path al archivo TOML: " + path_id);

        const auto root = toml::parse(CONFIG_PATH + data.paths.at(path_id));

        return toml::find<T>(root, sub_category);
    }

public:
    static GameConfig& get();

    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;

    const ArchetypeData& get_archetype(uint8_t id) const;

    const RaceData& get_race(uint8_t id) const;

    const RaceData& get_creature(uint8_t id) const;

    uint8_t get_creature_base_level(uint8_t id) const;

    const VariationData& get_variation(uint8_t id) const;

    const CooldownData& get_player_cooldown() const;

    const CooldownData& get_creature_cooldown() const;

    const EquipableItemData& get_equipable(uint8_t id) const;

    const UsableItemData& get_usable(uint8_t id) const;

    const WeaponData& get_weapon(uint8_t id) const;

    const DropProbabilitiesData& get_drop_probabilities() const;

    uint16_t get_item_price(uint8_t item_id) const;

    bool usables_contains(uint8_t id) const;

    bool weapons_contains(uint8_t id) const;

    bool helmets_contains(uint8_t id) const;

    bool armors_contains(uint8_t id) const;

    bool shields_contains(uint8_t id) const;

    uint8_t get_min_usable_id() const;

    uint8_t get_max_usable_id() const;

    uint8_t get_min_equipable_id() const;

    uint8_t get_max_equipable_id() const;

    const std::vector<uint8_t>& get_priest_items(int id) const;

    int get_priest_max_id() const;

    const std::vector<uint8_t>& get_merchant_items(int id) const;

    int get_merchant_max_id() const;
};


#endif  // GAME_CONFIG_H
