#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H


#include <memory>

#include "common/dto/events/event.h"

#include "client_receiver.h"
#include "client_sender.h"

class ConnectionHandler {
    Socket socket;

    ClientSender sender;
    ClientReceiver receiver;

public:
    explicit ConnectionHandler(Socket&& socket);

    void start();

    void stop();

    ClientMapDataDTO receive_map();

    bool try_pop_snapshot(SnapshotDTO& snapshot);

    SnapshotDTO pop_snapshot();

    void push_command(std::unique_ptr<EventDTO>&& event);

    bool is_finished();
};


#endif  // CONNECTION_HANDLER_H
