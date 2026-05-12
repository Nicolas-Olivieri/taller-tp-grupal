#include "client.h"

#define CLI_HOSTNAME argv[1]
#define CLI_SERVNAME argv[2]
#define CLI_MIN_ARGS 3


int main(const int argc, char* argv[]) {
    if (argc != CLI_MIN_ARGS) {
        return 1;
    }

    Client cliente(CLI_HOSTNAME, CLI_SERVNAME);
    cliente.run();

    return 0;
}
