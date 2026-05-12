#include "../common/socket.h"
#include "client.h"

#define HOSTNAME argv[1]
#define SERVNAME argv[2]

int main(const int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    Client cliente(Socket(HOSTNAME, SERVNAME));
    cliente.run();

    return 0;
}