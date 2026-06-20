#include "deserializer.h"

#include <functional>
#include <map>
#include <stdexcept>

#include <arpa/inet.h>

#include "common/dto/snapshot/actions/action_types/act_list_items/list_items.h"

Deserializer::Deserializer(Socket& socket): socket(socket) {}

std::string Deserializer::recv_string() {
    uint16_t size = recv_uint16();

    std::string bytes(size, 0);
    if (size > 0)
        this->socket.recvall(&bytes[0], size);

    return bytes;
}

uint8_t Deserializer::recv_uint8() {
    uint8_t byte;
    this->socket.recvall(&byte, sizeof(byte));

    return byte;
}

uint16_t Deserializer::recv_uint16() {
    uint16_t bytes;
    this->socket.recvall(&bytes, sizeof(bytes));

    return ntohs(bytes);
}

uint32_t Deserializer::recv_uint32() {
    uint32_t bytes;
    this->socket.recvall(&bytes, sizeof(bytes));

    return ntohl(bytes);
}

CommandType Deserializer::recv_command_type() {
    uint8_t byte = recv_uint8();

    switch (static_cast<CommandType>(byte)) {
        // TODO: agregar un case para todos los tipos de comandos existentes,
        // luego borrar este comentario
        case CommandType::MOVE:
        case CommandType::INTERACT:
        case CommandType::CHAT:
        case CommandType::RESURRECT:
        case CommandType::HEAL:
        case CommandType::LIST_ITEMS:
        case CommandType::BUY_ITEM:
        case CommandType::SELL_ITEM:
        case CommandType::DEPOSIT_ITEM:
        case CommandType::WITHDRAW_ITEM:
        case CommandType::DEPOSIT_GOLD:
        case CommandType::WITHDRAW_GOLD:
        case CommandType::PICKUP:
        case CommandType::USE_ITEM:
        case CommandType::DROP_ITEM:
        case CommandType::UNEQUIP_ITEM:
        case CommandType::CLAN_FOUND:
        case CommandType::CLAN_JOIN:
        case CommandType::CLAN_REQUEST_RESPONSE:
        case CommandType::CLAN_REMOVE_PLAYER:
        case CommandType::CLAN_LEAVE:
        case CommandType::CLAN_REVIEW:
        case CommandType::CHEAT_XP:
        case CommandType::CHEAT_GOLD:
        case CommandType::CHEAT_DEATH:
        case CommandType::CHEAT_INFINITE_RECOVERABLES:
        case CommandType::CHEAT_ITEM:
        case CommandType::CHEAT_KILL_CREATURES:
        case CommandType::MEDITATE:
        case CommandType::TELEPORT:
        case CommandType::INVENTORY_INFO:
            return static_cast<CommandType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de comando no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

Direction Deserializer::recv_direction() {
    uint8_t byte = recv_uint8();

    switch (static_cast<Direction>(byte)) {
        case Direction::DOWN:
        case Direction::UP:
        case Direction::LEFT:
        case Direction::RIGHT:
        case Direction::IDLE:
            return static_cast<Direction>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de dirección no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

std::vector<PlayerInfoDTO> Deserializer::recv_players_information() {
    uint16_t size = recv_uint16();

    std::vector<PlayerInfoDTO> players_information;
    players_information.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        players_information.push_back(recv_player_info());
    }

    return players_information;
}

std::vector<CreatureInfoDTO> Deserializer::recv_creatures_information() {
    uint16_t size = recv_uint16();

    std::vector<CreatureInfoDTO> creature_information;
    creature_information.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        creature_information.push_back(recv_creature_info());
    }

    return creature_information;
}

std::vector<LootInfoDTO> Deserializer::recv_loot_information() {
    uint16_t size = recv_uint16();

    std::vector<LootInfoDTO> loot_information;
    loot_information.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        loot_information.push_back(recv_loot_info());
    }

    return loot_information;
}

PlayerInfoDTO Deserializer::recv_player_info() {
    std::string name = recv_string();
    ClanInfoDTO clan = recv_clan();
    Direction direction = recv_direction();
    uint16_t x = recv_uint16();
    uint16_t y = recv_uint16();
    uint16_t safe_gold = recv_uint16();
    uint16_t excess_gold = recv_uint16();
    AppearanceDTO appearance = recv_appearance();
    PlayerStatsDTO stats = recv_player_stats();
    InventoryInfoDTO inventory = recv_inventory_info();
    EquipmentInfoDTO equipment = recv_equipment_info();

    return PlayerInfoDTO(name, clan, direction, x, y, safe_gold, excess_gold, appearance, stats, inventory,
                         equipment);
}

CreatureInfoDTO Deserializer::recv_creature_info() {
    uint8_t creature_id = recv_uint8();
    uint8_t variation_id = recv_uint8();
    uint16_t sub_id = recv_uint16();
    Direction direction = recv_direction();
    uint16_t x = recv_uint16();
    uint16_t y = recv_uint16();
    CreatureStatsDTO stats = recv_creature_stats();

    return CreatureInfoDTO(creature_id, variation_id, sub_id, direction, x, y, stats);
}

CreatureStatsDTO Deserializer::recv_creature_stats() {
    const uint16_t max_health = recv_uint16();
    const uint16_t current_health = recv_uint16();
    const uint8_t xp_level = recv_uint8();

    return CreatureStatsDTO(max_health, current_health, xp_level);
}

LootInfoDTO Deserializer::recv_loot_info() {
    LootType type = recv_loot_type();
    uint16_t x = recv_uint16();
    uint16_t y = recv_uint16();

    return LootInfoDTO(type, x, y);
}

std::vector<ActionDTO> Deserializer::recv_actions() {
    uint16_t size = recv_uint16();

    std::vector<ActionDTO> actions;
    actions.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        actions.push_back(recv_action());
    }

    return actions;
}

ActionDTO Deserializer::recv_action() {
    ActionType type = recv_action_type();
    // TODO: recordar modificar esto para recibir la información según el
    // ActionType
    switch (type) {
        case ActionType::ATTACK:
            return ActionDTO(recv_attack());
        case ActionType::DESPAWN:
            return ActionDTO(recv_despawn());
        case ActionType::HEAL:
            return ActionDTO(recv_heal());
        case ActionType::MEDITATION:
            return ActionDTO(recv_meditation());
        case ActionType::MESSAGE:
            return ActionDTO(recv_chat_message());
        case ActionType::RESURRECTION:
            return ActionDTO(recv_resurrection());
        case ActionType::DEATH:
            return ActionDTO(recv_death());
        case ActionType::MESSAGE_LIST:
            return ActionDTO(recv_chat_list());
        case ActionType::LIST_ITEMS:
            return ActionDTO(recv_list_items());
        case ActionType::LIST_BANK:
            return ActionDTO(recv_list_bank());
        case ActionType::CLAN_MESSAGE:
            return ActionDTO(recv_clan_message());
        case ActionType::CLAN_ACCEPT:
            return ActionDTO(recv_clan_accept());
        case ActionType::CLAN_FOUND:
            return ActionDTO(recv_clan_found());
        case ActionType::CLAN_LEAVE:
            return ActionDTO(recv_clan_leave());
        case ActionType::INVENTORY_LIST:
            return ActionDTO(recv_inventory_list());
        default:
            throw std::runtime_error("Deserializer encontró un tipo de acción desconocido");
    }
}

ActionType Deserializer::recv_action_type() {
    uint8_t byte = recv_uint8();

    switch (static_cast<ActionType>(byte)) {
        // TODO: agregar un case para todos los tipos de comandos existentes,
        // luego borrar este comentario
        case ActionType::ATTACK:
        case ActionType::DESPAWN:
        case ActionType::HEAL:
        case ActionType::MEDITATION:
        case ActionType::MESSAGE:
        case ActionType::RESURRECTION:
        case ActionType::DEATH:
        case ActionType::MESSAGE_LIST:
        case ActionType::LIST_ITEMS:
        case ActionType::LIST_BANK:
        case ActionType::CLAN_MESSAGE:
        case ActionType::CLAN_ACCEPT:
        case ActionType::CLAN_FOUND:
        case ActionType::CLAN_LEAVE:
        case ActionType::INVENTORY_LIST:
            return static_cast<ActionType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de acción no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

AppearanceDTO Deserializer::recv_appearance() {
    // TODO 1: recordar considerar equipamientos y eso en el futuro
    // TODO 2: capaz en un futuro cada categoría debería ser un enum
    uint8_t body = recv_uint8();
    uint8_t head = recv_uint8();

    return AppearanceDTO(body, head);
}

ClanInfoDTO Deserializer::recv_clan() {
    const std::string name = recv_string();
    const uint8_t is_founder = recv_uint8();

    return ClanInfoDTO(name, is_founder);
}

DespawnDTO Deserializer::recv_despawn() {
    std::string player_despawned = recv_string();

    return DespawnDTO(player_despawned);
}

HealDTO Deserializer::recv_heal() {
    const std::string player_healed = recv_string();

    return HealDTO(player_healed);
}

std::vector<AllyInfoDTO> Deserializer::recv_allies_information() {
    const uint16_t size = recv_uint16();

    std::vector<AllyInfoDTO> allies_information;
    allies_information.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        allies_information.push_back(recv_ally_info());
    }

    return allies_information;
}

AllyInfoDTO Deserializer::recv_ally_info() {
    const AllyType type = recv_ally_type();
    const uint16_t x = recv_uint16();
    const uint16_t y = recv_uint16();

    return AllyInfoDTO(type, x, y);
}

AllyType Deserializer::recv_ally_type() {
    const uint8_t byte = recv_uint8();

    switch (static_cast<AllyType>(byte)) {
        case AllyType::PRIEST:
        case AllyType::MERCHANT:
        case AllyType::BANKER:
        case AllyType::TOTEM:
            return static_cast<AllyType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de aliado no reconocido");
    }
}

ChatMessageDTO Deserializer::recv_chat_message() {
    MessageType type = recv_message_type();
    std::string sender = recv_string();
    std::string receiver = recv_string();
    std::string content = recv_string();

    return ChatMessageDTO(type, sender, receiver, content);
}

MessageType Deserializer::recv_message_type() {
    uint8_t byte = recv_uint8();

    switch (static_cast<MessageType>(byte)) {
        // TODO: agregar un case para todos los tipos de comandos existentes,
        // luego borrar este comentario
        case MessageType::SYSTEM:
        case MessageType::PRIVATE:
        case MessageType::GLOBAL:
        case MessageType::CLAN:
        case MessageType::ERROR:
        case MessageType::ALLY:
            return static_cast<MessageType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de visibilidad de mensaje no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

PlayerStatsDTO Deserializer::recv_player_stats() {
    const uint16_t max_health = recv_uint16();
    const uint16_t current_health = recv_uint16();
    const uint16_t max_mana = recv_uint16();
    const uint16_t current_mana = recv_uint16();
    const uint8_t xp_level = recv_uint8();
    const uint32_t current_xp_amount = recv_uint32();
    const uint32_t max_xp_amount = recv_uint32();

    return PlayerStatsDTO(max_health, current_health, max_mana, current_mana, xp_level, current_xp_amount,
                          max_xp_amount);
}

InventoryInfoDTO Deserializer::recv_inventory_info() {
    const uint16_t size = recv_uint16();
    std::map<uint8_t, uint8_t, std::greater<>> items;

    for (uint16_t i = 0; i < size; ++i) {
        const uint8_t item_id = recv_uint8();
        const uint8_t amount = recv_uint8();
        items[item_id] = amount;
    }

    return InventoryInfoDTO(items);
}

EquipableItemInfoDTO Deserializer::recv_equipable_item_info() {
    const uint8_t item_id = recv_uint8();
    const uint8_t effect = recv_uint8();

    return EquipableItemInfoDTO(item_id, effect);
}

EquipmentInfoDTO Deserializer::recv_equipment_info() {
    const EquipableItemInfoDTO weapon = recv_equipable_item_info();
    const EquipableItemInfoDTO shield = recv_equipable_item_info();
    const EquipableItemInfoDTO helmet = recv_equipable_item_info();
    const EquipableItemInfoDTO armor = recv_equipable_item_info();

    return EquipmentInfoDTO(weapon, shield, helmet, armor);
}

AttackDTO Deserializer::recv_attack() {
    const uint8_t weapon = recv_uint8();
    const uint16_t x = recv_uint16();
    const uint16_t y = recv_uint16();
    const uint8_t missed = recv_uint8();

    return AttackDTO(weapon, x, y, missed);
}

MeditationDTO Deserializer::recv_meditation() {
    const std::string player_meditating = recv_string();

    return MeditationDTO(player_meditating);
}

ResurrectionDTO Deserializer::recv_resurrection() {
    std::string name = recv_string();
    AppearanceDTO appearance = recv_appearance();

    return ResurrectionDTO(name, appearance);
}

DeathDTO Deserializer::recv_death() {
    const std::string name = recv_string();

    return DeathDTO(name);
}

ChatListDTO Deserializer::recv_chat_list() {
    MessageType type = recv_message_type();
    std::string receiver = recv_string();

    std::vector<std::string> lines;

    uint16_t size = recv_uint16();

    for (int i = 0; i < size; ++i) {
        lines.push_back(recv_string());
    }

    return ChatListDTO(type, lines, receiver);
}

ListBankDTO Deserializer::recv_list_bank() {
    const MessageType type = recv_message_type();
    const std::string receiver = recv_string();

    const uint16_t gold = recv_uint16();

    const uint16_t size = recv_uint16();
    std::map<uint8_t, uint16_t> items;

    for (uint16_t i = 0; i < size; ++i) {
        const uint8_t item_id = recv_uint8();
        const uint16_t amount = recv_uint16();
        items[item_id] = amount;
    }

    return ListBankDTO(type, gold, items, receiver);
}

ListItemsDTO Deserializer::recv_list_items() {
    MessageType type = recv_message_type();
    const std::string receiver = recv_string();

    const uint16_t size = recv_uint16();
    std::map<uint8_t, uint16_t> items;

    for (uint16_t i = 0; i < size; ++i) {
        const uint8_t item_id = recv_uint8();
        const uint16_t price = recv_uint16();
        items[item_id] = price;
    }

    return ListItemsDTO(type, items, receiver);
}

std::vector<AssetInfoDTO> Deserializer::recv_assets_information() {
    const uint16_t size = recv_uint16();

    std::vector<AssetInfoDTO> assets_information;
    assets_information.reserve(size);

    for (uint16_t i = 0; i < size; ++i) {
        assets_information.push_back(recv_asset_info());
    }

    return assets_information;
}

AssetInfoDTO Deserializer::recv_asset_info() {
    const uint8_t id = recv_uint8();
    const uint16_t x = recv_uint16();
    const uint16_t y = recv_uint16();

    return AssetInfoDTO(id, x, y);
}

ClanMessageDTO Deserializer::recv_clan_message() {
    const std::string receiver_clan = recv_string();
    const std::string content = recv_string();
    const std::string sender = recv_string();

    return ClanMessageDTO(receiver_clan, content, sender);
}

ClanAcceptDTO Deserializer::recv_clan_accept() {
    const std::string founder = recv_string();
    const std::string accepted = recv_string();

    return ClanAcceptDTO(founder, accepted);
}

ClanFoundDTO Deserializer::recv_clan_found() {
    const std::string founder = recv_string();

    return ClanFoundDTO(founder);
}

ClanLeaveDTO Deserializer::recv_clan_leave() {
    const std::string leaver = recv_string();

    return ClanLeaveDTO(leaver);
}

LootType Deserializer::recv_loot_type() {
    uint8_t byte = recv_uint8();

    switch (static_cast<LootType>(byte)) {
        case LootType::GOLD:
        case LootType::ITEM:
        case LootType::SECRET_ITEM:
            return static_cast<LootType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de looot no reconocido");
    }
}

InventoryListDTO Deserializer::recv_inventory_list() {
    const std::string player_name = recv_string();
    const InventoryInfoDTO inventory = recv_inventory_info();
    return InventoryListDTO(player_name, inventory);
}
