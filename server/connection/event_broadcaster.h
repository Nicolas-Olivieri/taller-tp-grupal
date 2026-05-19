#ifndef EVENT_BROADCASTER_H
#define EVENT_BROADCASTER_H

#include <list>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/snapshot.h"
#include "common/queue.h"
#include "server/game/player.h"

class EventBroadcaster {
private:
    std::mutex mtx;
    std::list<Queue<SnapshotDTO>*> client_queues;
    std::vector<ActionDTO> actions;

public:
    EventBroadcaster() = default;

    void add_queue(Queue<SnapshotDTO>& queue);

    void remove_queue(const Queue<SnapshotDTO>& queue);

    void broadcast(const std::unordered_map<std::string, Player>& players);

    void add_action(const ActionDTO& action);

    EventBroadcaster(const EventBroadcaster&) = delete;

    EventBroadcaster& operator=(const EventBroadcaster&) = delete;

    ~EventBroadcaster() = default;
};


#endif  // EVENT_BROADCASTER_H
