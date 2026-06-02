#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

#include "common/dto/events/chatevent.h"
#include "common/dto/events/event.h"
#include "common/dto/events/interact_event.h"
#include "common/dto/events/moveevent.h"
#include "common/dto/lobby/credentials.h"
#include "common/dto/message.h"
#include "common/dto/snapshot/snapshot.h"

class Serializer {
private:
    std::vector<uint8_t>& buffer;
    size_t offset;

    void serialize(const std::string& value);

    void serialize(uint8_t value);

    void serialize(uint16_t value);

    // Generaliza la forma de serializar vectores de cualquier tipo T
    template <typename T>
    void serialize(const std::vector<T>& container) {
        serialize(static_cast<uint16_t>(container.size()));
        for (const auto& elem: container) serialize(elem);
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

    void serialize(const PlayerInfoDTO& info);

    void serialize(const ActionDTO& action);

    void serialize(const AppearanceDTO& appearance);

    void serialize(const DespawnDTO& despawn);

    void serialize(const InteractEventDTO& event);

    void serialize(const ChatMessageDTO& message);

    void serialize(const ChatEventDTO& event);
};

#endif  // SERIALIZER_H
