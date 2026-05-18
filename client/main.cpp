#include <iostream>

#include "common/logger.h"

#include "client.h"

#define CLI_MIN_ARGS 3

#define ERROR 1


int main(const int argc, char* argv[]) {
    const auto log = Logger();
    try {

        if (argc != CLI_MIN_ARGS) {
            std::cerr << "Usage: " << argv[0] << " " << std::endl;
            return ERROR;
        }

        // TODO probar meter el init de QT app aca
        //  hay que emular los argc y argv para hacerlo, y no parece ser lo más
        //  conveniente tmpc
        //        int _argc = 3;
        //        char *_argv[] = {argv[0], &text1, "", nullptr};
        //        QApplication app(argc, argv);

        Client client(argc, argv);
        return client.run();
    } catch (const std::exception& e) {
        log.err(EXCEPTION_MSG, e);
    } catch (...) {
        log.crit(UNKNOWN_EXCEPTION_MSG);
    }

    return ERROR;
}
