#include "deserializer.h"

#include <map>
#include <stdexcept>
#include <unordered_map>

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
            return static_cast<CommandType>(byte);
        default:  // Undefined Behavior -> Excepción
            throw std::invalid_argument("Byte de comando no reconocido");
            // TODO: chequear si es la mejor excepción
    }
}

Direction Deserializer::recv_direction() {
    uint8_t byte = recv_uint8();

    switch (static_cast<Direction>(byte)) {
        // TODO: agregar un case para todos los tipos de comandos existentes,
        // luego borrar este comentario
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
    std::string clan_name = recv_string();
    Direction direction = recv_direction();
    uint16_t x = recv_uint16();
    uint16_t y = recv_uint16();
    uint16_t safe_gold = recv_uint16();
    uint16_t excess_gold = recv_uint16();
    AppearanceDTO appearance = recv_appearance();
    PlayerStatsDTO stats = recv_player_stats();
    InventoryInfoDTO inventory = recv_inventory_info();
    EquipmentInfoDTO equipment = recv_equipment_info();

    return PlayerInfoDTO(name, clan_name, direction, x, y, safe_gold, excess_gold, appearance, stats,
                         inventory, equipment);
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
    bool is_item = recv_uint8();
    uint16_t x = recv_uint16();
    uint16_t y = recv_uint16();

    return LootInfoDTO(is_item, x, y);
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
        case ActionType::MESSAGE:
        case ActionType::RESURRECTION:
        case ActionType::DEATH:
        case ActionType::MESSAGE_LIST:
        case ActionType::LIST_ITEMS:
        case ActionType::LIST_BANK:
        case ActionType::CLAN_MESSAGE:
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
    std::unordered_map<uint8_t, uint8_t> items;

    for (uint16_t i = 0; i < size; ++i) {
        const uint8_t item_id = recv_uint8();
        const uint8_t amount = recv_uint8();
        items[item_id] = amount;
    }

    return InventoryInfoDTO(items);
}

EquipmentInfoDTO Deserializer::recv_equipment_info() {
    const uint8_t weapon = recv_uint8();
    const uint8_t shield = recv_uint8();
    const uint8_t helmet = recv_uint8();
    const uint8_t armor = recv_uint8();

    return EquipmentInfoDTO(weapon, shield, helmet, armor);
}

AttackDTO Deserializer::recv_attack() {
    const std::string attacker = recv_string();
    const uint8_t weapon = recv_uint8();

    return AttackDTO(attacker, weapon);
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
