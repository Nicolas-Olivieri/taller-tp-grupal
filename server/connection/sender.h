#ifndef SENDER_H
#define SENDER_H

#include "common/protocol/protocol.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"

#include "broadcast_register.h"


class Sender: public Thread {
private:
    Protocol protocol;
    Queue<SnapshotDTO>& client_queue;
    BroadcastRegister broadcast;

public:
    Sender(Socket& peer, Queue<SnapshotDTO>& client_queue, EventBroadcaster& broadcaster);

    void run() override;
};


#endif  // SENDER_H
