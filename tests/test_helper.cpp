#include "test_helper.h"

#include <functional>
#include <map>
#include <string>
#include <vector>

TestHelper::TestHelper() = default;

std::vector<AssetInfoDTO> TestHelper::mock_asset_vector(const int size) {
    std::vector<AssetInfoDTO> result;
    for (int i = 0; i < size; i++) {
        AssetInfoDTO asset(i, i, i);
        result.push_back(asset);
    }
    return result;
}

AppearanceDTO TestHelper::mock_appearance() { return AppearanceDTO(1, 2); }

ClanInfoDTO TestHelper::mock_clan_info() { return ClanInfoDTO("TestClan", 0); }

PlayerStatsDTO TestHelper::mock_player_stats() { return PlayerStatsDTO(100, 80, 50, 30, 5, 1200, 2000); }

EquipableItemInfoDTO TestHelper::mock_equipable_item() { return EquipableItemInfoDTO(1, 5); }

EquipmentInfoDTO TestHelper::mock_equipment() {
    return EquipmentInfoDTO(mock_equipable_item(), mock_equipable_item(), mock_equipable_item(),
                            mock_equipable_item());
}

InventoryInfoDTO TestHelper::mock_inventory_info() {
    std::map<uint8_t, uint8_t, std::greater<>> items;
    items[1] = 3;
    items[2] = 1;
    items[3] = 7;
    return InventoryInfoDTO(items);
}

PlayerInfoDTO TestHelper::mock_player_info() {
    return PlayerInfoDTO("dummy", mock_clan_info(), Direction::IDLE, 0, 0, 0, 0, mock_appearance(),
                         mock_player_stats(), mock_inventory_info(), mock_equipment());
}

CredentialsDTO TestHelper::mock_credentials(const std::string& username) { return CredentialsDTO(username); }

ExistenceDTO TestHelper::mock_existence(uint8_t exists, uint8_t connected) {
    return ExistenceDTO(exists, connected);
}

CreatePlayerDTO TestHelper::mock_create_player() { return CreatePlayerDTO(1, 1, 1, 1); }

CreatureStatsDTO TestHelper::mock_creature_stats() { return CreatureStatsDTO(50, 30, 2); }

CreatureInfoDTO TestHelper::mock_creature_info() {
    return CreatureInfoDTO(1, 1, 100, Direction::UP, 10, 20, mock_creature_stats());
}

LootInfoDTO TestHelper::mock_loot_info() { return LootInfoDTO(LootType::GOLD, 15, 15); }

bool TestHelper::equals(const AssetInfoDTO& a, const AssetInfoDTO& b) {
    return a.id == b.id && a.x == b.x && a.y == b.y;
}

bool TestHelper::equals(const std::vector<AssetInfoDTO>& a, const std::vector<AssetInfoDTO>& b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!equals(a[i], b[i]))
            return false;
    }
    return true;
}

bool TestHelper::equals(const AppearanceDTO& a, const AppearanceDTO& b) {
    return a.body == b.body && a.head == b.head;
}

bool TestHelper::equals(const ClanInfoDTO& a, const ClanInfoDTO& b) {
    return a.name == b.name && a.is_founder == b.is_founder;
}

bool TestHelper::equals(const PlayerStatsDTO& a, const PlayerStatsDTO& b) {
    return a.max_health == b.max_health && a.current_health == b.current_health && a.max_mana == b.max_mana &&
           a.current_mana == b.current_mana && a.xp_level == b.xp_level &&
           a.current_xp_amount == b.current_xp_amount && a.max_xp_amount == b.max_xp_amount;
}

bool TestHelper::equals(const EquipableItemInfoDTO& a, const EquipableItemInfoDTO& b) {
    return a.item_id == b.item_id && a.effect == b.effect;
}

bool TestHelper::equals(const EquipmentInfoDTO& a, const EquipmentInfoDTO& b) {
    return equals(a.weapon, b.weapon) && equals(a.shield, b.shield) && equals(a.helmet, b.helmet) &&
           equals(a.armor, b.armor);
}

bool TestHelper::equals(const InventoryInfoDTO& a, const InventoryInfoDTO& b) {
    if (a.items.size() != b.items.size())
        return false;
    for (const auto& [key, val]: a.items) {
        auto it = b.items.find(key);
        if (it == b.items.end() || it->second != val)
            return false;
    }
    return true;
}

bool TestHelper::equals(const PlayerInfoDTO& a, const PlayerInfoDTO& b) {
    return a.name == b.name && equals(a.clan, b.clan) && a.direction == b.direction && a.x == b.x &&
           a.y == b.y && a.safe_gold == b.safe_gold && a.excess_gold == b.excess_gold &&
           equals(a.appearance, b.appearance) && equals(a.stats, b.stats) &&
           equals(a.inventory, b.inventory) && equals(a.equipment, b.equipment);
}

bool TestHelper::equals(const std::vector<PlayerInfoDTO>& a, const std::vector<PlayerInfoDTO>& b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!equals(a[i], b[i]))
            return false;
    }
    return true;
}

bool TestHelper::equals(const CredentialsDTO& a, const CredentialsDTO& b) { return a.username == b.username; }

bool TestHelper::equals(const ExistenceDTO& a, const ExistenceDTO& b) {
    return a.user_exists == b.user_exists && a.user_connected == b.user_connected;
}

bool TestHelper::equals(const CreatePlayerDTO& a, const CreatePlayerDTO& b) {
    return equals(a.appearance, b.appearance) && a.archetype == b.archetype && a.race == b.race;
}

bool TestHelper::equals(const CreatureStatsDTO& a, const CreatureStatsDTO& b) {
    return a.max_health == b.max_health && a.current_health == b.current_health && a.xp_level == b.xp_level;
}

bool TestHelper::equals(const CreatureInfoDTO& a, const CreatureInfoDTO& b) {
    return a.creature == b.creature && a.variation == b.variation && a.sub_id == b.sub_id &&
           a.direction == b.direction && a.x == b.x && a.y == b.y && equals(a.stats, b.stats);
}

bool TestHelper::equals(const std::vector<CreatureInfoDTO>& a, const std::vector<CreatureInfoDTO>& b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!equals(a[i], b[i]))
            return false;
    }
    return true;
}

bool TestHelper::equals(const LootInfoDTO& a, const LootInfoDTO& b) {
    return a.type == b.type && a.x == b.x && a.y == b.y;
}

bool TestHelper::equals(const std::vector<LootInfoDTO>& a, const std::vector<LootInfoDTO>& b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!equals(a[i], b[i]))
            return false;
    }
    return true;
}
