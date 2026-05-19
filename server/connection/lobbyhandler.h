#ifndef _LOBBYHANDLER_H
#define _LOBBYHANDLER_H

#include "common/socket.h"
#include "common/thread/thread.h"

class LobbyHandler: public Thread {
private:
    Socket socket;

public:
    explicit LobbyHandler(Socket&& socket);

    void run() override;

    ~LobbyHandler();
};

#endif  // _LOBBYHANDLER_H
