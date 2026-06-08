
#include "snapshot_builder.h"

SnapshotDTO SnapshotBuilder::build() const {
    return SnapshotDTO(players_info, creatures_info, loot_info, actions);
}

void SnapshotBuilder::add_players(const std::unordered_map<std::string, Player>& players) {
    for (const auto& [player_name, player]: players) {
        players_info.push_back(convert_to_info(player_name, player));
    }
}

void SnapshotBuilder::add_creatures(const std::unordered_map<uint16_t, Creature>& creatures) {
    for (const auto& [sub_id, creature]: creatures) {
        creatures_info.push_back(convert_to_info(sub_id, creature));
    }
}

void SnapshotBuilder::add_loot(const std::map<std::pair<uint16_t, uint16_t>, Tile*>& lootable_tiles) {
    for (const auto& [position, tile]: lootable_tiles) {
        loot_info.push_back(convert_to_info(position, *tile));
    }
}

void SnapshotBuilder::add_action(const ActionDTO& action) { actions.push_back(action); }

PlayerInfoDTO SnapshotBuilder::convert_to_info(const std::string& player_name, const Player& player) {
    Position position = player.get_position();
    Stats stats = player.get_stats();
    Equipment equipment = player.get_equipment();

    return PlayerInfoDTO(
            player_name, player.get_direction(), position.get_x(), position.get_y(), player.get_safe_gold(),
            player.get_excess_gold(), AppearanceDTO(player.get_body(), player.get_head()),
            PlayerStatsDTO(stats.health.get_max(), stats.health.get_current(), stats.mana.get_max(),
                           stats.mana.get_current(), stats.experience.get_level(),
                           stats.experience.get_current_amount(),
                           Calculator::calculate_xp_limit(stats.experience.get_level())),
            InventoryInfoDTO(player.get_inventory_items()),
            EquipmentInfoDTO(equipment.weapon, equipment.shield, equipment.helmet, equipment.armor));
}

CreatureInfoDTO SnapshotBuilder::convert_to_info(uint16_t sub_id, const Creature& creature) {
    Position position = creature.get_position();
    Stats stats = creature.get_stats();

    return CreatureInfoDTO(stats.race_id, stats.archetype_id, sub_id, creature.get_direction(),
                           position.get_x(), position.get_y());
}

LootInfoDTO SnapshotBuilder::convert_to_info(const std::pair<uint16_t, uint16_t>& position, Tile& tile) {
    return LootInfoDTO(tile.get_loot().top().type == LootType::ITEM, position.first, position.second);
}
