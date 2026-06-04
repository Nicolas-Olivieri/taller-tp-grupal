#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "common/dto/events/buy_event.h"
#include "common/dto/events/chatevent.h"
#include "common/dto/events/event.h"
#include "common/dto/events/interact_event.h"
#include "common/dto/events/moveevent.h"
#include "common/dto/events/sell_event.h"
#include "common/dto/lobby/ally_info.h"
#include "common/dto/lobby/create_player.h"
#include "common/dto/lobby/credentials.h"
#include "common/dto/lobby/existence.h"
#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_list/chat_list.h"
#include "common/dto/snapshot/actions/action_types/act_list_items/list_items.h"
#include "common/dto/snapshot/actions/action_types/act_resurrection/resurrection.h"
#include "common/dto/snapshot/info/player_stats.h"
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

public:
    explicit Serializer(std::vector<uint8_t>& buffer);  // NOLINT

    void serialize(const ProtocolMessageDTO& dto);

    // Las siguientes son para aplicar double dispatch con los DTO que heredan
    // de ProtocolMessageDTO
    void serialize(const CredentialsDTO& credentials);

    void serialize(const EventDTO& event);

    void serialize(const MoveEventDTO& event);

    void serialize(const SnapshotDTO& snapshot);

    void serialize(const ExistenceDTO& existence);

    void serialize(const CreatePlayerDTO& player_data);

    void serialize(const PlayerInfoDTO& info);

    // Es el que se sigue expandiendo al agregar una action nueva
    void serialize(const ActionDTO& action);

    void serialize(const AppearanceDTO& appearance);

    void serialize(const DespawnDTO& despawn);

    void serialize(const InteractEventDTO& event);

    void serialize(const ChatMessageDTO& message);

    void serialize(const ChatEventDTO& event);

    void serialize(const AllyInfoDTO& info);

    void serialize(const PlayerStatsDTO& stats);

    void serialize(const ResurrectionDTO& resurrection);

    void serialize(const DeathDTO& death);

    void serialize(const ChatListDTO& list);

    void serialize(const ListItemsDTO& list);

    void serialize(const BuyEventDTO& event);

    void serialize(const SellEventDTO& event);
};

#endif  // SERIALIZER_H
