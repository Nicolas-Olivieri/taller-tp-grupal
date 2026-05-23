#ifndef RECEIVER_H
#define RECEIVER_H

#include <memory>
#include <string>

#include "common/protocol/protocol.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"
#include "server/command/command.h"
#include "server/command/command_factory.h"


class Receiver: public Thread {
private:
    Protocol protocol;
    std::string player_name;
    CommandFactory factory;
    Queue<std::unique_ptr<Command>>& command_queue;

public:
    Receiver(Socket& peer, const std::string& player_name, Queue<std::unique_ptr<Command>>& command_queue);

    void run() override;
};


#endif  // RECEIVER_H
