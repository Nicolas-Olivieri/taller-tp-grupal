#include "snapshot.h"

#include <numeric>

#include "common/protocol/serializer.h"

SnapshotDTO::SnapshotDTO(const std::vector<PlayerInfoDTO>& players_information,
                         const std::vector<ActionDTO>& actions):
        players_information(players_information), actions(actions) {}

size_t SnapshotDTO::message_size() const {
    size_t size = 0;

    size += sizeof(uint16_t);
    size_t players_size = std::accumulate(
            players_information.begin(), players_information.end(), size,
            [](size_t accumulated, const auto& player) {
                return accumulated + player.message_size();
            });

    size += sizeof(uint16_t);
    size_t actions_size =
            std::accumulate(actions.begin(), actions.end(), size,
                            [](size_t accumulated, const auto& action) {
                                return accumulated + action.message_size();
                            });

    return size + players_size + actions_size;
}

void SnapshotDTO::accept(Serializer& serializer) const {
    serializer.serialize(*this);
}
