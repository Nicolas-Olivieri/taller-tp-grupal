#include "event_broadcaster.h"

#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"
#include "common/dto/snapshot/snapshot.h"


void EventBroadcaster::add_queue(Queue<SnapshotDTO>& queue) {
    std::unique_lock lock(mtx);
    client_queues.push_back(&queue);
}


void EventBroadcaster::remove_queue(const Queue<SnapshotDTO>& queue) {
    std::unique_lock lock(mtx);
    client_queues.remove_if(
            [&queue](const Queue<SnapshotDTO>* q) { return q == &queue; });
}


void EventBroadcaster::broadcast(
        const std::unordered_map<std::string, Player>& players) {
    std::unique_lock lock(mtx);

    std::vector<PlayerInfoDTO> players_info;
    for (const auto& [player_name, player]: players) {
        Position position = player.get_position();
        players_info.emplace_back(player_name, player.get_direction(),
                                  position.get_x(), position.get_y());
    }

    const SnapshotDTO snapshot(players_info, actions);
    for (const auto& queue: client_queues) {
        queue->try_push(snapshot);
    }
}


void EventBroadcaster::add_action(const ActionDTO& action) {
    actions.push_back(action);
}
