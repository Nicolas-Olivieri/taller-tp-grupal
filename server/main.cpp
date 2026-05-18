#include "common/logger.h"

#include "server.h"

#define SERV_SERVNAME argv[1]
#define SERV_MIN_ARGS 2

#define SUCCESS 0
#define ERROR 1


int main(const int argc, const char* argv[]) {
    const auto log = Logger();

    try {
        if (argc != SERV_MIN_ARGS && argv[0] != nullptr) {
            return ERROR;
        }

        log.open(SERV_SERVNAME);

        Server server;
        server.run();

        return SUCCESS;

    } catch (const std::exception& e) {
        log.err(EXCEPTION_MSG, e);

    } catch (...) {
        log.crit(UNKNOWN_EXCEPTION_MSG);
    }

    return ERROR;
}
