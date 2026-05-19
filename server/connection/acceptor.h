#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include <memory>
#include <string>
#include <vector>

#include "common/socket.h"
#include "common/thread/thread.h"

#include "lobbyhandler.h"

class Acceptor: public Thread {
private:
    Socket listener;
    std::vector<std::unique_ptr<LobbyHandler>> lobbies;

    void wait_for_player();

    void reap_lobbies();

    void send_to_lobby(Socket&& peer);

public:
    explicit Acceptor(const std::string& servname);

    void run() override;

    ~Acceptor();
};

#endif  // _ACCEPTOR_H
