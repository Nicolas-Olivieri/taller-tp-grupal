#ifndef CLIENT_H
#define CLIENT_H

#include "common/logger.h"


class Client {

public:
    Client();

    void run(const Logger& log);
};


#endif  // CLIENT_H
