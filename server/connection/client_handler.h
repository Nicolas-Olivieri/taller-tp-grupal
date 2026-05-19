#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <memory>
#include <string>

#include "common/queue.h"
#include "common/socket.h"
#include "server/command/command.h"

#include "receiver.h"
#include "sender.h"


class ClientHandler {
private:
    // límite de mensajes encolados para cada cliente
    static constexpr int CLIENT_QUEUE_MAX_SIZE = 512;

    Socket peer;
    const std::string player_name;
    Queue<SnapshotDTO> client_queue;
    Sender sender;
    Receiver receiver;

public:
    explicit ClientHandler(Socket&& peer, const std::string& player_name,
                           Queue<std::unique_ptr<Command>>& command_queue,
                           EventBroadcaster& broadcaster);

    void start();

    void stop();

    void join();

    bool is_alive() const;

    const std::string& get_name() const;

    void polite_kill();

    void hard_kill();

    ClientHandler(const ClientHandler&) = delete;

    ClientHandler& operator=(const ClientHandler&) = delete;

    ~ClientHandler();
};


#endif  // CLIENT_HANDLER_H
