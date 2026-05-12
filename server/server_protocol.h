#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H
#include "common/socket.h"
#include "common/types.h"

class ServerProtocol {
private:
    Socket peer;

public:
    explicit ServerProtocol(Socket&& peer);

    void send_msg(const DataDTO& data);

    DataDTO recv_msg();
};


#endif
