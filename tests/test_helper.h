#ifndef TEST_HELPER_H
#define TEST_HELPER_H
#include <string>
#include <vector>

#include "common/dto/lobby/create_player.h"
#include "common/dto/lobby/credentials.h"
#include "common/dto/lobby/existence.h"
#include "common/dto/snapshot/info/appearance.h"
#include "common/dto/snapshot/info/clan_info.h"
#include "common/dto/snapshot/info/creature_stats.h"
#include "common/dto/snapshot/info/equipable_item_info.h"
#include "common/dto/snapshot/info/equipment_info.h"
#include "common/dto/snapshot/info/inventory_info.h"
#include "common/dto/snapshot/info/player_stats.h"
#include "common/dto/snapshot/info/playerinfo.h"
#include "common/dto/snapshot/map/asset_info.h"
#include "common/dto/snapshot/snapshot.h"


class TestHelper {
public:
    TestHelper();

    PlayerInfoDTO mock_player_info();

    AppearanceDTO mock_appearance();

    ClanInfoDTO mock_clan_info();

    PlayerStatsDTO mock_player_stats();

    EquipableItemInfoDTO mock_equipable_item();

    EquipmentInfoDTO mock_equipment();

    InventoryInfoDTO mock_inventory_info();

    ExistenceDTO mock_existence(uint8_t exists, uint8_t connected);

    CreatePlayerDTO mock_create_player();

    CreatureStatsDTO mock_creature_stats();

    CreatureInfoDTO mock_creature_info();

    LootInfoDTO mock_loot_info();

    CredentialsDTO mock_credentials(const std::string& username);

    std::vector<AssetInfoDTO> mock_asset_vector(int size);

    bool equals(const PlayerInfoDTO& a, const PlayerInfoDTO& b);

    bool equals(const std::vector<PlayerInfoDTO>& a, const std::vector<PlayerInfoDTO>& b);

    bool equals(const ClanInfoDTO& a, const ClanInfoDTO& b);

    bool equals(const PlayerStatsDTO& a, const PlayerStatsDTO& b);

    bool equals(const EquipableItemInfoDTO& a, const EquipableItemInfoDTO& b);

    bool equals(const EquipmentInfoDTO& a, const EquipmentInfoDTO& b);

    bool equals(const InventoryInfoDTO& a, const InventoryInfoDTO& b);

    bool equals(const ExistenceDTO& a, const ExistenceDTO& b);

    bool equals(const CreatePlayerDTO& a, const CreatePlayerDTO& b);

    bool equals(const CreatureStatsDTO& a, const CreatureStatsDTO& b);

    bool equals(const std::vector<CreatureInfoDTO>& a, const std::vector<CreatureInfoDTO>& b);

    bool equals(const AssetInfoDTO& a, const AssetInfoDTO& b);

    bool equals(const std::vector<AssetInfoDTO>& a, const std::vector<AssetInfoDTO>& b);

    bool equals(const LootInfoDTO& a, const LootInfoDTO& b);

    bool equals(const std::vector<LootInfoDTO>& a, const std::vector<LootInfoDTO>& b);

    bool equals(const CreatureInfoDTO& a, const CreatureInfoDTO& b);

    bool equals(const CredentialsDTO& a, const CredentialsDTO& b);

    bool equals(const AppearanceDTO& a, const AppearanceDTO& b);
};


#endif  // TEST_HELPER_H
