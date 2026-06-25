#include "test_helper.h"

#include <format>
#include <functional>
#include <map>
#include <string>
#include <vector>

TestHelper::TestHelper() = default;

// MOCK DE INFO :::::::::::::

std::vector<AssetInfoDTO> TestHelper::mock_asset_vector(const int size) {
    std::vector<AssetInfoDTO> result;
    for (int i = 0; i < size; i++) {
        AssetInfoDTO asset(i, i, i);
        result.push_back(asset);
    }
    return result;
}

MoveEventDTO TestHelper::mock_move_event() { return MoveEventDTO(Direction::DOWN); }

ChatEventDTO TestHelper::mock_chat_event() { return ChatEventDTO("dummy", "content"); }

EventDTO TestHelper::mock_event(const CommandType& command) { return EventDTO(command); }

BuyEventDTO TestHelper::mock_buy_event() { return BuyEventDTO(1); }

DepositGoldEventDTO TestHelper::mock_deposit_gold_event() { return DepositGoldEventDTO(1000); }

ClanFoundEventDTO TestHelper::mock_clan_found_event() { return ClanFoundEventDTO("TestClan"); }

RequestResponseEventDTO TestHelper::mock_request_response_event() {
    return RequestResponseEventDTO("TestClan", true);
}

InteractEventDTO TestHelper::mock_interact_event() { return InteractEventDTO(150, 200); }

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

PlayerInfoDTO TestHelper::mock_player_info(int variation) {
    return PlayerInfoDTO(std::format("dummy_{}", variation), mock_clan_info(), Direction::IDLE, 1 * variation,
                         1 * variation, 100 * variation, 50 * variation, mock_appearance(),
                         mock_player_stats(), mock_inventory_info(), mock_equipment());
}

std::vector<PlayerInfoDTO> TestHelper::mock_players_information(const int size) {
    std::vector<PlayerInfoDTO> result;
    for (int i = 0; i < size; i++) {
        result.push_back(mock_player_info(i));
    }
    return result;
}

CredentialsDTO TestHelper::mock_credentials(const std::string& username) { return CredentialsDTO(username); }

ExistenceDTO TestHelper::mock_existence() { return ExistenceDTO(1, 0); }

CreatePlayerDTO TestHelper::mock_create_player() { return CreatePlayerDTO(1, 1, 1, 1); }

CreatureStatsDTO TestHelper::mock_creature_stats() { return CreatureStatsDTO(50, 30, 2); }

CreatureInfoDTO TestHelper::mock_creature_info(const int variation) {
    return CreatureInfoDTO(1 * variation, 1 * variation, 100 * variation, Direction::UP, 10 * variation,
                           20 * variation, mock_creature_stats());
}

std::vector<CreatureInfoDTO> TestHelper::mock_creatures_information(const int size) {
    std::vector<CreatureInfoDTO> result;
    for (int i = 0; i < size; i++) {
        result.push_back(mock_creature_info(i));
    }
    return result;
}

LootInfoDTO TestHelper::mock_loot_info(const int variation) {
    return LootInfoDTO(LootType::GOLD, 15 * variation, 15 * variation);
}

std::vector<LootInfoDTO> TestHelper::mock_loots_information(const int size) {
    std::vector<LootInfoDTO> result;
    for (int i = 0; i < size; i++) {
        result.push_back(mock_loot_info(i));
    }
    return result;
}


// MOCK DE ACTIONS :::::::::::::

AttackDTO TestHelper::mock_attack() { return AttackDTO(1, 5, 5, 0); }

DespawnDTO TestHelper::mock_despawn() { return DespawnDTO("DespawnedPlayer"); }

HealDTO TestHelper::mock_heal() { return HealDTO("HealedPlayer"); }

ChatMessageDTO TestHelper::mock_chat_message() {
    return ChatMessageDTO(MessageType::GLOBAL, "sender", "receiver", "Hello world");
}

MeditationDTO TestHelper::mock_meditation() { return MeditationDTO("MeditatingPlayer"); }

ResurrectionDTO TestHelper::mock_resurrection() {
    return ResurrectionDTO("ResurrectedPlayer", mock_appearance());
}

DeathDTO TestHelper::mock_death() { return DeathDTO("DeadPlayer"); }

ChatListDTO TestHelper::mock_chat_list() {
    std::vector<std::string> lines = {"Line1", "Line2"};
    return ChatListDTO(MessageType::PRIVATE, lines, "ReceiverPlayer");
}

ListItemsDTO TestHelper::mock_list_items() {
    std::map<uint8_t, uint16_t> items;
    items[1] = 100;
    items[2] = 200;
    return ListItemsDTO(MessageType::SYSTEM, items, "Receiver");
}

ListBankDTO TestHelper::mock_list_bank() {
    std::map<uint8_t, uint16_t> items;
    items[3] = 50;
    items[4] = 150;
    return ListBankDTO(MessageType::SYSTEM, 1000, items, "BankReceiver");
}

ClanMessageDTO TestHelper::mock_clan_message() { return ClanMessageDTO("ClanName", "Content", "SenderName"); }

ClanFoundDTO TestHelper::mock_clan_found() { return ClanFoundDTO("FounderName"); }

ClanAcceptDTO TestHelper::mock_clan_accept() { return ClanAcceptDTO("Founder", "AcceptedPlayer"); }

ClanLeaveDTO TestHelper::mock_clan_leave() { return ClanLeaveDTO("LeaverPlayer"); }

InventoryListDTO TestHelper::mock_inventory_list() {
    return InventoryListDTO("PlayerName", mock_inventory_info());
}

std::vector<ActionDTO> TestHelper::mock_actions() {
    std::vector<ActionDTO> result;

    result.emplace_back(mock_attack());
    result.emplace_back(mock_despawn());
    result.emplace_back(mock_heal());
    result.emplace_back(mock_chat_message());
    result.emplace_back(mock_meditation());
    result.emplace_back(mock_resurrection());
    result.emplace_back(mock_death());
    result.emplace_back(mock_chat_list());
    result.emplace_back(mock_list_items());
    result.emplace_back(mock_list_bank());
    result.emplace_back(mock_clan_message());
    result.emplace_back(mock_clan_found());
    result.emplace_back(mock_clan_accept());
    result.emplace_back(mock_clan_leave());
    result.emplace_back(mock_inventory_list());

    return result;
}


// EQUALS DE INFO :::::::::::::

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

bool TestHelper::equals(const RequestedCommandDTO& a, const MoveEventDTO& b) {
    return a.command == b.command && a.direction == b.direction;
}

bool TestHelper::equals(const RequestedCommandDTO& a, const ChatEventDTO& b) {
    return a.command == b.command && a.receiver == b.receiver && a.message == b.content;
}

bool TestHelper::equals(const RequestedCommandDTO& a, const EventDTO& b) { return a.command == b.command; }

bool TestHelper::equals(const RequestedCommandDTO& a, const BuyEventDTO& b) {
    return a.command == b.command && a.one_byte_number == b.item_id;
}

bool TestHelper::equals(const RequestedCommandDTO& a, const DepositGoldEventDTO& b) {
    return a.command == b.command && a.gold_amount == b.gold_amount;
}

bool TestHelper::equals(const RequestedCommandDTO& a, const ClanFoundEventDTO& b) {
    return a.command == b.command && a.clan_name == b.clan_name;
}

bool TestHelper::equals(const RequestedCommandDTO& a, const RequestResponseEventDTO& b) {
    return a.command == b.command && a.player_name == b.player_name && a.command_selector == b.is_accepted;
}

bool TestHelper::equals(const RequestedCommandDTO& a, const InteractEventDTO& b) {
    return a.command == b.command && a.x == b.target_x && a.y == b.target_y;
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

// EQUALS DE ACTIONS :::::::::::::

bool TestHelper::equals(const AttackDTO& a, const AttackDTO& b) {
    return a.weapon == b.weapon && a.x == b.x && a.y == b.y && a.missed == b.missed;
}

bool TestHelper::equals(const DespawnDTO& a, const DespawnDTO& b) {
    return a.player_despawned == b.player_despawned;
}

bool TestHelper::equals(const HealDTO& a, const HealDTO& b) { return a.player_healed == b.player_healed; }


bool TestHelper::equals(const ChatMessageDTO& a, const ChatMessageDTO& b) {
    return a.type == b.type && a.sender == b.sender && a.receiver == b.receiver && a.content == b.content;
}

bool TestHelper::equals(const MeditationDTO& a, const MeditationDTO& b) {
    return a.player_meditating == b.player_meditating;
}

bool TestHelper::equals(const ResurrectionDTO& a, const ResurrectionDTO& b) {
    return a.player_resurrected == b.player_resurrected &&
           equals(a.original_appearance, b.original_appearance);
}

bool TestHelper::equals(const DeathDTO& a, const DeathDTO& b) { return a.player_dead == b.player_dead; }

bool TestHelper::equals(const ChatListDTO& a, const ChatListDTO& b) {
    if (a.type != b.type || a.receiver != b.receiver || a.lines.size() != b.lines.size())
        return false;
    for (size_t i = 0; i < a.lines.size(); ++i) {
        if (a.lines[i] != b.lines[i])
            return false;
    }
    return true;
}

bool TestHelper::equals(const ListItemsDTO& a, const ListItemsDTO& b) {
    if (a.type != b.type || a.receiver != b.receiver || a.items.size() != b.items.size())
        return false;
    for (const auto& [key, val]: a.items) {
        auto it = b.items.find(key);
        if (it == b.items.end() || it->second != val)
            return false;
    }
    return true;
}

bool TestHelper::equals(const ListBankDTO& a, const ListBankDTO& b) {
    if (a.type != b.type || a.receiver != b.receiver || a.gold != b.gold || a.items.size() != b.items.size())
        return false;
    for (const auto& [key, val]: a.items) {
        auto it = b.items.find(key);
        if (it == b.items.end() || it->second != val)
            return false;
    }
    return true;
}

bool TestHelper::equals(const ClanMessageDTO& a, const ClanMessageDTO& b) {
    return a.receiver_clan == b.receiver_clan && a.content == b.content && a.sender == b.sender;
}

bool TestHelper::equals(const ClanFoundDTO& a, const ClanFoundDTO& b) { return a.founder == b.founder; }

bool TestHelper::equals(const ClanAcceptDTO& a, const ClanAcceptDTO& b) {
    return a.founder == b.founder && a.accepted == b.accepted;
}

bool TestHelper::equals(const ClanLeaveDTO& a, const ClanLeaveDTO& b) { return a.leaver == b.leaver; }

bool TestHelper::equals(const InventoryListDTO& a, const InventoryListDTO& b) {
    return a.player_name == b.player_name && equals(a.inventory, b.inventory);
}

bool TestHelper::equals(const ActionDTO& a, const ActionDTO& b) {
    if (a.action != b.action)
        return false;
    switch (a.action) {
        case ActionType::ATTACK:
            return equals(a.attack, b.attack);
        case ActionType::DESPAWN:
            return equals(a.despawn, b.despawn);
        case ActionType::HEAL:
            return equals(a.heal, b.heal);
        case ActionType::MEDITATION:
            return equals(a.meditation, b.meditation);
        case ActionType::MESSAGE:
            return equals(a.chat_message, b.chat_message);
        case ActionType::RESURRECTION:
            return equals(a.resurrection, b.resurrection);
        case ActionType::DEATH:
            return equals(a.death, b.death);
        case ActionType::MESSAGE_LIST:
            return equals(a.list, b.list);
        case ActionType::LIST_ITEMS:
            return equals(a.items, b.items);
        case ActionType::LIST_BANK:
            return equals(a.bank, b.bank);
        case ActionType::CLAN_MESSAGE:
            return equals(a.clan_msg, b.clan_msg);
        case ActionType::CLAN_ACCEPT:
            return equals(a.clan_accept, b.clan_accept);
        case ActionType::CLAN_FOUND:
            return equals(a.clan_found, b.clan_found);
        case ActionType::CLAN_LEAVE:
            return equals(a.clan_leave, b.clan_leave);
        case ActionType::INVENTORY_LIST:
            return equals(a.inventory_list, b.inventory_list);
        default:
            return false;
    }
}

bool TestHelper::equals(const std::vector<ActionDTO>& a, const std::vector<ActionDTO>& b) {
    if (a.size() != b.size())
        return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (!equals(a[i], b[i]))
            return false;
    }
    return true;
}
