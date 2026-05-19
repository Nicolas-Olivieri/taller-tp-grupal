#ifndef SENDER_H
#define SENDER_H

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"


class Sender: public Thread {
private:
    Socket& peer;
    Queue<int>& client_queue;

public:
    Sender(Socket& peer, Queue<int>& client_queue);

    void run() override;
};


#endif  // SENDER_H
