#ifndef TEST_HELPER_H
#define TEST_HELPER_H
#include <string>
#include <vector>

#include "common/dto/events/ally_related/deposit/deposit_gold_event.h"
#include "common/dto/events/ally_related/interact_event.h"
#include "common/dto/events/ally_related/shop/buy_event.h"
#include "common/dto/events/chat/chatevent.h"
#include "common/dto/events/clan/clan_found_event.h"
#include "common/dto/events/clan/clan_request_response_event.h"
#include "common/dto/events/movement/moveevent.h"
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
private:
    CreatureInfoDTO mock_creature_info(int variation);
    PlayerInfoDTO mock_player_info(int variation);
    LootInfoDTO mock_loot_info(int variation);

public:
    TestHelper();

    // MOCK DE INFO :::::::::::::
    AppearanceDTO mock_appearance();
    ClanInfoDTO mock_clan_info();
    PlayerStatsDTO mock_player_stats();
    EquipableItemInfoDTO mock_equipable_item();
    EquipmentInfoDTO mock_equipment();
    InventoryInfoDTO mock_inventory_info();
    ExistenceDTO mock_existence();
    CreatePlayerDTO mock_create_player();
    CreatureStatsDTO mock_creature_stats();
    CredentialsDTO mock_credentials(const std::string& username);

    std::vector<AssetInfoDTO> mock_asset_vector(int size);
    std::vector<PlayerInfoDTO> mock_players_information(int size);
    std::vector<CreatureInfoDTO> mock_creatures_information(int size);
    std::vector<LootInfoDTO> mock_loots_information(int size);

    // MOCK DE ACTIONS :::::::::::::
    std::vector<ActionDTO> mock_actions();
    AttackDTO mock_attack();
    DespawnDTO mock_despawn();
    HealDTO mock_heal();
    ChatMessageDTO mock_chat_message();
    MeditationDTO mock_meditation();
    ResurrectionDTO mock_resurrection();
    DeathDTO mock_death();
    ChatListDTO mock_chat_list();
    ListItemsDTO mock_list_items();
    ListBankDTO mock_list_bank();
    ClanMessageDTO mock_clan_message();
    ClanFoundDTO mock_clan_found();
    ClanAcceptDTO mock_clan_accept();
    ClanLeaveDTO mock_clan_leave();
    InventoryListDTO mock_inventory_list();

    // MOCK DE EVENTS :::::::::::::
    MoveEventDTO mock_move_event();
    ChatEventDTO mock_chat_event();
    EventDTO mock_event(const CommandType& command);
    BuyEventDTO mock_buy_event();
    DepositGoldEventDTO mock_deposit_gold_event();
    ClanFoundEventDTO mock_clan_found_event();
    RequestResponseEventDTO mock_request_response_event();
    InteractEventDTO mock_interact_event();

    // EQUALS DE INFO :::::::::::::
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

    // EQUALS DE ACTIONS :::::::::::::
    bool equals(const AttackDTO& a, const AttackDTO& b);
    bool equals(const DespawnDTO& a, const DespawnDTO& b);
    bool equals(const HealDTO& a, const HealDTO& b);
    bool equals(const ChatMessageDTO& a, const ChatMessageDTO& b);
    bool equals(const MeditationDTO& a, const MeditationDTO& b);
    bool equals(const ResurrectionDTO& a, const ResurrectionDTO& b);
    bool equals(const DeathDTO& a, const DeathDTO& b);
    bool equals(const ChatListDTO& a, const ChatListDTO& b);
    bool equals(const ListItemsDTO& a, const ListItemsDTO& b);
    bool equals(const ListBankDTO& a, const ListBankDTO& b);
    bool equals(const ClanMessageDTO& a, const ClanMessageDTO& b);
    bool equals(const ClanFoundDTO& a, const ClanFoundDTO& b);
    bool equals(const ClanAcceptDTO& a, const ClanAcceptDTO& b);
    bool equals(const ClanLeaveDTO& a, const ClanLeaveDTO& b);
    bool equals(const InventoryListDTO& a, const InventoryListDTO& b);
    bool equals(const ActionDTO& a, const ActionDTO& b);
    bool equals(const std::vector<ActionDTO>& a, const std::vector<ActionDTO>& b);

    // EQUALS DE EVENTS :::::::::::::
    bool equals(const RequestedCommandDTO& a, const MoveEventDTO& b);
    bool equals(const RequestedCommandDTO& a, const ChatEventDTO& b);
    bool equals(const RequestedCommandDTO& a, const EventDTO& b);
    bool equals(const RequestedCommandDTO& a, const BuyEventDTO& b);
    bool equals(const RequestedCommandDTO& a, const DepositGoldEventDTO& b);
    bool equals(const RequestedCommandDTO& a, const ClanFoundEventDTO& b);
    bool equals(const RequestedCommandDTO& a, const RequestResponseEventDTO& b);
    bool equals(const RequestedCommandDTO& a, const InteractEventDTO& b);
};


#endif  // TEST_HELPER_H
