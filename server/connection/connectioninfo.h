#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <string>

#include "common/socket.h"

struct ConnectionInfo {
    std::string username;
    Socket socket;
};

#endif  // CONNECTIONINFO_H
