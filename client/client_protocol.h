#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H
#include <common/socket.h>

#include "common/types.h"


class ClientProtocol {
private:
    Socket skt;

public:
    explicit ClientProtocol(Socket&& skt);

    void send_msg(const DataDTO& data);

    DataDTO recv_msg();
};


#endif
