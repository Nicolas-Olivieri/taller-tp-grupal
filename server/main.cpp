#include "common/socket.h"
#include "server.h"

#define SERVNAME argv[1]

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    Server server((Socket(SERVNAME)));
    server.run();

    return 0;
}
