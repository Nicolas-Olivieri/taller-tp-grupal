#ifndef RECEIVER_H
#define RECEIVER_H

#include <memory>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"
#include "server/command/command.h"


class Receiver: public Thread {
private:
    Socket& peer;
    Queue<std::unique_ptr<Command>>& command_queue;

public:
    Receiver(Socket& peer, Queue<std::unique_ptr<Command>>& command_queue);

    void run() override;
};


#endif  // RECEIVER_H
