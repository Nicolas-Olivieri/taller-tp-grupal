#ifndef _LOBBYHANDLER_H
#define _LOBBYHANDLER_H

#include <mutex>
#include <string>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"

#include "connectioninfo.h"

class LobbyHandler: public Thread {
private:
    std::mutex mutex;
    bool handshake_finished;
    Socket socket;
    Queue<ConnectionInfo>& waiting_players;

    void move_into_waiting_queue(const std::string& username);

    void shutdown_if_blocked();
public:
    LobbyHandler(Socket&& socket, Queue<ConnectionInfo>& waiting_players);

    void run() override;

    ~LobbyHandler();
};

#endif  // _LOBBYHANDLER_H
