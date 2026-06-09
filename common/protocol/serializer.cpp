#include "serializer.h"

#include <cstring>
#include <stdexcept>

#include <arpa/inet.h>

#include "common/dto/events/deposit_gold_event.h"
#include "common/dto/lobby/existence.h"
#include "common/dto/snapshot/info/inventory_info.h"

Serializer::Serializer(std::vector<uint8_t>& buffer): buffer(buffer), offset(0) {}

// Double dispatch para definir cómo se serializa cada DTO
void Serializer::serialize(const ProtocolMessageDTO& dto) { dto.accept(*this); }

void Serializer::serialize(const CredentialsDTO& credentials) {
    serialize(static_cast<uint8_t>(Message::CREDENTIALS));
    serialize(credentials.username);
    serialize(credentials.password);
}

void Serializer::serialize(const EventDTO& event) {
    serialize(static_cast<uint8_t>(Message::COMMAND));
    serialize(static_cast<uint8_t>(event.command));
}

void Serializer::serialize(const MoveEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(static_cast<uint8_t>(event.direction));
}

void Serializer::serialize(const ExistenceDTO& existence) {
    serialize(static_cast<uint8_t>(Message::EXISTENCE));
    serialize(existence.user_exists);
    serialize(existence.user_connected);
}

void Serializer::serialize(const CreatePlayerDTO& player_data) {
    serialize(static_cast<uint8_t>(Message::CREATE_PLAYER));
    serialize(player_data.appearance);
    serialize(player_data.archetype);
    serialize(player_data.race);
}

void Serializer::serialize(const ClientMapDataDTO& map) {
    serialize(static_cast<uint8_t>(Message::MAP));
    serialize(map.world_width);
    serialize(map.world_height);
    serialize(map.tiles);
    serialize(map.colliders);
    serialize(map.npcs);
}

void Serializer::serialize(const AssetInfoDTO& asset) {
    serialize(asset.id);
    serialize(asset.x);
    serialize(asset.y);
}

void Serializer::serialize(const SnapshotDTO& snapshot) {
    serialize(static_cast<uint8_t>(Message::SNAPSHOT));
    serialize(snapshot.players_information);
    serialize(snapshot.creatures_information);
    serialize(snapshot.loot_information);
    serialize(snapshot.actions);
}

void Serializer::serialize(const PlayerInfoDTO& info) {
    serialize(info.name);
    serialize(static_cast<uint8_t>(info.direction));
    serialize(info.x);
    serialize(info.y);
    serialize(info.safe_gold);
    serialize(info.excess_gold);
    serialize(info.appearance);
    serialize(info.stats);
    serialize(info.inventory);
    serialize(info.equipment);
}

void Serializer::serialize(const CreatureInfoDTO& info) {
    serialize(info.creature);
    serialize(info.variation);
    serialize(info.sub_id);
    serialize(static_cast<uint8_t>(info.direction));
    serialize(info.x);
    serialize(info.y);
}

void Serializer::serialize(const LootInfoDTO& info) {
    serialize(static_cast<uint8_t>(info.is_item));
    serialize(info.x);
    serialize(info.y);
}

// TODO: se debería serializar dependiendo de action.action (ActionType)
void Serializer::serialize(const ActionDTO& action) {
    serialize(static_cast<uint8_t>(action.action));

    switch (action.action) {
        case ActionType::ATTACK:
            serialize(action.attack);
            break;
        case ActionType::DESPAWN:
            serialize(action.despawn);
            break;
        case ActionType::HEAL:
            serialize(action.heal);
            break;
        case ActionType::MESSAGE:
            serialize(action.chat_message);
            break;
        case ActionType::RESURRECTION:
            serialize(action.resurrection);
            break;
        case ActionType::DEATH:
            serialize(action.death);
            break;
        case ActionType::MESSAGE_LIST:
            serialize(action.list);
            break;
        case ActionType::LIST_ITEMS:
            serialize(action.items);
            break;
        case ActionType::LIST_BANK:
            serialize(action.bank);
            break;
        default:
            throw std::runtime_error("Serializer encontró un tipo de acción desconocido");
    }
}

void Serializer::serialize(const AppearanceDTO& appearance) {
    serialize(appearance.body);
    serialize(appearance.head);
}

void Serializer::serialize(const std::string& value) {
    uint16_t size = static_cast<uint16_t>(value.size());
    serialize(size);

    copy_to_buffer(value.data(), size);
}

void Serializer::serialize(uint8_t value) { this->buffer[this->offset++] = value; }

void Serializer::serialize(uint16_t value) {
    uint16_t netvalue = ntohs(value);

    copy_to_buffer(&netvalue, sizeof(netvalue));
}

void Serializer::serialize(uint32_t value) {
    uint32_t netvalue = ntohl(value);

    copy_to_buffer(&netvalue, sizeof(netvalue));
}

void Serializer::copy_to_buffer(const void* data, size_t size) {
    std::memcpy(&this->buffer[this->offset], data, size);
    offset += size;
}

void Serializer::serialize(const InteractEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.target_x);
    serialize(event.target_y);
}

void Serializer::serialize(const DespawnDTO& despawn) { serialize(despawn.player_despawned); }

void Serializer::serialize(const HealDTO& heal) { serialize(heal.player_healed); }

void Serializer::serialize(const AllyInfoDTO& info) {
    serialize(static_cast<uint8_t>(info.type));
    serialize(info.x);
    serialize(info.y);
}

void Serializer::serialize(const ChatMessageDTO& message) {
    serialize(static_cast<uint8_t>(message.type));
    serialize(message.sender);
    serialize(message.receiver);
    serialize(message.content);
}

void Serializer::serialize(const ChatEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.receiver);
    serialize(event.content);
}

void Serializer::serialize(const PlayerStatsDTO& stats) {
    serialize(stats.max_health);
    serialize(stats.current_health);
    serialize(stats.max_mana);
    serialize(stats.current_mana);
    serialize(stats.xp_level);
    serialize(stats.current_xp_amount);
    serialize(stats.max_xp_amount);
}

void Serializer::serialize(const InventoryInfoDTO& inventory) { serialize(inventory.items); }

void Serializer::serialize(const EquipmentInfoDTO& equipment) {
    serialize(equipment.weapon);
    serialize(equipment.shield);
    serialize(equipment.helmet);
    serialize(equipment.armor);
}

void Serializer::serialize(const AttackDTO& attack) {
    serialize(attack.attacker);
    serialize(attack.weapon);
}

void Serializer::serialize(const ResurrectionDTO& resurrection) {
    serialize(resurrection.player_resurrected);
    serialize(resurrection.original_appearance);
}

void Serializer::serialize(const DeathDTO& death) { serialize(death.player_dead); }

void Serializer::serialize(const ChatListDTO& list) {
    serialize(static_cast<uint8_t>(list.type));
    serialize(list.receiver);
    serialize(list.lines);
}

void Serializer::serialize(const ListBankDTO& bank) {
    serialize(static_cast<uint8_t>(bank.type));
    serialize(bank.receiver);
    serialize(bank.gold);
    serialize(bank.items);
}

void Serializer::serialize(const ListItemsDTO& list) {
    serialize(static_cast<uint8_t>(list.type));
    serialize(list.receiver);
    serialize(list.items);
}

void Serializer::serialize(const BuyEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.item_id);
}

void Serializer::serialize(const SellEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.item_id);
}

void Serializer::serialize(const DepositItemEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.item_id);
}

void Serializer::serialize(const WithdrawItemEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.item_id);
}

void Serializer::serialize(const DepositGoldEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.gold_amount);
}

void Serializer::serialize(const WithdrawGoldEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.gold_amount);
}

void Serializer::serialize(const UseItemEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.item_id);
}

void Serializer::serialize(const DropItemEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.item_id);
}

void Serializer::serialize(const UnequipItemEventDTO& event) {
    serialize(EventDTO(event.command));
    serialize(event.item_id);
}
