#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H


#include <memory>

#include "common/dto/events/event.h"
#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"

class ClientSender: public Thread {

    Socket& socket;

    Queue<std::unique_ptr<EventDTO>> sender_q;

    void run() override;

public:
    explicit ClientSender(Socket& socket);


    void push(std::unique_ptr<EventDTO>&& event);

    void close();
};


#endif  // CLIENT_SENDER_H
