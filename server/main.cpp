#include "common/socket.h"

#include "server.h"

#define SERV_SERVNAME argv[1]
#define SERV_MIN_ARGS 2

int main(const int argc, const char* argv[]) {
    if (argc != SERV_MIN_ARGS && argv[0] != nullptr) {
        return 1;
    }


    Server server;
    server.run();

    return 0;
}
