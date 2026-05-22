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
    client_queues.remove_if([&queue](const Queue<SnapshotDTO>* q) { return q == &queue; });
}


void EventBroadcaster::broadcast(const SnapshotDTO& snapshot) {
    std::unique_lock lock(mtx);

    for (const auto& queue: client_queues) {
        queue->try_push(snapshot);
    }
}
