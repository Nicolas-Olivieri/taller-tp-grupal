#include "snapshot.h"

#include <numeric>

#include "common/protocol/serializer.h"

SnapshotDTO::SnapshotDTO(const std::vector<PlayerInfoDTO>& players_information,
                         const std::vector<CreatureInfoDTO>& creatures_information,
                         const std::vector<LootInfoDTO>& loot_information,
                         const std::vector<ActionDTO>& actions):
        players_information(players_information),
        creatures_information(creatures_information),
        loot_information(loot_information),
        actions(actions) {}

size_t SnapshotDTO::message_size() const {
    size_t size = 0;

    size += sizeof(uint16_t);
    size_t players_size = std::accumulate(
            players_information.begin(), players_information.end(), 0,
            [](size_t accumulated, const auto& player) { return accumulated + player.message_size(); });

    size += sizeof(uint16_t);
    size_t creatures_size = std::accumulate(
            creatures_information.begin(), creatures_information.end(), 0,
            [](size_t accumulated, const auto& creature) { return accumulated + creature.message_size(); });

    size += sizeof(uint16_t);
    size_t loot_size = std::accumulate(
            loot_information.begin(), loot_information.end(), 0,
            [](size_t accumulated, const auto& creature) { return accumulated + creature.message_size(); });

    size += sizeof(uint16_t);
    size_t actions_size = std::accumulate(
            actions.begin(), actions.end(), 0,
            [](size_t accumulated, const auto& action) { return accumulated + action.message_size(); });

    return size + creatures_size + players_size + loot_size + actions_size;
}

void SnapshotDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
