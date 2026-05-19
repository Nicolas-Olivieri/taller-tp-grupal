#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include <memory>
#include <string>
#include <vector>

#include "common/queue.h"
#include "common/socket.h"
#include "common/thread/thread.h"

#include "connectioninfo.h"
#include "lobbyhandler.h"

class Acceptor: public Thread {
private:
    Socket listener;
    std::vector<std::unique_ptr<LobbyHandler>> lobbies;
    Queue<ConnectionInfo>& waiting_players;

    void wait_for_player();

    void reap_lobbies();

    void send_to_lobby(Socket&& peer);

public:
    Acceptor(const std::string& servname, Queue<ConnectionInfo>& waiting_players);

    void run() override;

    ~Acceptor();
};

#endif  // _ACCEPTOR_H
