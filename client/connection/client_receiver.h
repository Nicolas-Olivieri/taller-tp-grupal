#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H


#include "common/dto/snapshot/snapshot.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"

class ClientReceiver: public Thread {
    Socket& socket;
    Queue<SnapshotDTO> receiver_q;

    void run() override;

public:
    explicit ClientReceiver(Socket& socket);


    bool try_pop(SnapshotDTO& snapshot);
};


#endif  // CLIENT_RECEIVER_H
