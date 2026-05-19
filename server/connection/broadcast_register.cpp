#include "broadcast_register.h"


BroadcastRegister::BroadcastRegister(EventBroadcaster& broadcaster,
                                     Queue<SnapshotDTO>& queue):
        broadcaster(broadcaster), queue(queue) {
    broadcaster.add_queue(queue);
}


BroadcastRegister::~BroadcastRegister() { broadcaster.remove_queue(queue); }
