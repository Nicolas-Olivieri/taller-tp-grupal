#ifndef BROADCAST_REGISTER_H
#define BROADCAST_REGISTER_H

#include <vector>

#include "common/queue.h"

#include "event_broadcaster.h"


class BroadcastRegister {
private:
    EventBroadcaster& broadcaster;
    Queue<SnapshotDTO>& queue;

public:
    explicit BroadcastRegister(EventBroadcaster& broadcaster,
                               Queue<SnapshotDTO>& queue);

    ~BroadcastRegister();
};


#endif  // BROADCAST_REGISTER_H
