#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/dto/events/ally_related/deposit/deposit_gold_event.h"
#include "common/dto/events/ally_related/deposit/deposit_item_event.h"
#include "common/dto/events/ally_related/interact_event.h"
#include "common/dto/events/ally_related/shop/buy_event.h"
#include "common/dto/events/ally_related/shop/sell_event.h"
#include "common/dto/events/ally_related/withdraw/withdraw_gold_event.h"
#include "common/dto/events/ally_related/withdraw/withdraw_item_event.h"
#include "common/dto/events/chat/chatevent.h"
#include "common/dto/events/clan/clan_found_event.h"
#include "common/dto/events/clan/clan_join_event.h"
#include "common/dto/events/clan/clan_remove_player_event.h"
#include "common/dto/events/clan/clan_request_response_event.h"
#include "common/dto/events/drop_item_event.h"
#include "common/dto/events/event.h"
#include "common/dto/events/movement/moveevent.h"
#include "common/dto/events/unequip_item_event.h"
#include "common/dto/events/use_item_event.h"
#include "common/dto/lobby/ally_info.h"
#include "common/dto/lobby/create_player.h"
#include "common/dto/lobby/credentials.h"
#include "common/dto/lobby/existence.h"
#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_list/chat_list.h"
#include "common/dto/snapshot/actions/action_types/act_list_items/list_items.h"
#include "common/dto/snapshot/actions/action_types/act_resurrection/resurrection.h"
#include "common/dto/snapshot/info/inventory_info.h"
#include "common/dto/snapshot/info/player_stats.h"
#include "common/dto/snapshot/map/client_map_data.h"
#include "common/dto/snapshot/snapshot.h"

class Serializer {
private:
    std::vector<uint8_t>& buffer;
    size_t offset;

    void serialize(const std::string& value);

    void serialize(uint8_t value);

    void serialize(uint16_t value);

    void serialize(uint32_t value);

    void copy_to_buffer(const void* data, size_t size);

    // Generaliza la forma de serializar vectores de cualquier tipo T
    template <typename T>
    void serialize(const std::vector<T>& container) {
        serialize(static_cast<uint16_t>(container.size()));
        for (const auto& elem: container) serialize(elem);
    }

    // Generaliza la forma de serializar mapas con claves K y valores V
    template <typename K, typename V>
    void serialize(const std::map<K, V>& container) {
        serialize(static_cast<uint16_t>(container.size()));
        for (const auto& [key, value]: container) {
            serialize(key);
            serialize(value);
        }
    }

    template <typename K, typename V>
    void serialize(const std::unordered_map<K, V>& container) {
        serialize(static_cast<uint16_t>(container.size()));
        for (const auto& [key, value]: container) {
            serialize(key);
            serialize(value);
        }
    }

public:
    explicit Serializer(std::vector<uint8_t>& buffer);  // NOLINT

    void serialize(const ProtocolMessageDTO& dto);

    // Las siguientes son para aplicar double dispatch con los DTO que heredan
    // de ProtocolMessageDTO
    void serialize(const CredentialsDTO& credentials);

    void serialize(const EventDTO& event);

    void serialize(const MoveEventDTO& event);

    void serialize(const AssetInfoDTO& asset);

    void serialize(const ClientMapDataDTO& map);

    void serialize(const SnapshotDTO& snapshot);

    void serialize(const ExistenceDTO& existence);

    void serialize(const CreatePlayerDTO& player_data);

    void serialize(const PlayerInfoDTO& info);

    void serialize(const CreatureInfoDTO& info);

    void serialize(const LootInfoDTO& info);

    // Es el que se sigue expandiendo al agregar una action nueva
    void serialize(const ActionDTO& action);

    void serialize(const AppearanceDTO& appearance);

    void serialize(const DespawnDTO& despawn);

    void serialize(const InteractEventDTO& event);

    void serialize(const ChatMessageDTO& message);

    void serialize(const ChatEventDTO& event);

    void serialize(const AllyInfoDTO& info);

    void serialize(const PlayerStatsDTO& stats);

    void serialize(const InventoryInfoDTO& inventory);

    void serialize(const EquipmentInfoDTO& equipment);

    void serialize(const ResurrectionDTO& resurrection);

    void serialize(const DeathDTO& death);

    void serialize(const ChatListDTO& list);

    void serialize(const ListBankDTO& bank);

    void serialize(const ListItemsDTO& list);

    void serialize(const BuyEventDTO& event);

    void serialize(const SellEventDTO& event);

    void serialize(const DepositItemEventDTO& event);

    void serialize(const WithdrawItemEventDTO& event);

    void serialize(const DepositGoldEventDTO& event);

    void serialize(const WithdrawGoldEventDTO& event);

    void serialize(const UseItemEventDTO& event);

    void serialize(const DropItemEventDTO& event);

    void serialize(const UnequipItemEventDTO& event);

    void serialize(const ClanFoundEventDTO& event);

    void serialize(const ClanJoinEventDTO& event);

    void serialize(const RequestResponseEventDTO& event);

    void serialize(const ClanRemovePlayerEventDTO& event);
};

#endif  // SERIALIZER_H
