#include "common/logger.h"

#include "client.h"

#define CLI_HOSTNAME argv[1]
#define CLI_SERVNAME argv[2]
#define CLI_MIN_ARGS 3

#define SUCCESS 0
#define ERROR 1


int main(const int argc, const char* argv[]) {
    const auto log = Logger();

    try {
        if (argc != CLI_MIN_ARGS && argv[0] != nullptr) {
            return ERROR;
        }

        log.open(CLI_HOSTNAME, CLI_SERVNAME);

        Client client;
        client.run(log);

        return SUCCESS;

    } catch (const std::exception& e) {
        log.err(EXCEPTION_MSG, e);

    } catch (...) {
        log.crit(UNKNOWN_EXCEPTION_MSG);
    }

    return ERROR;
}
