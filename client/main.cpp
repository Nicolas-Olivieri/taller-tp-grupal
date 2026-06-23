#include <iostream>

#include "common/logger.h"

#include "client.h"

#define CLI_MIN_ARGS 3

#define ERROR 1


int main(const int argc, char* argv[]) {
    try {
        if (argc != CLI_MIN_ARGS) {
            std::cerr << "Corre el programa con el lanzador client.sh" << std::endl;
            return ERROR;
        }

        Client client(argc, argv);
        return client.run();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION_MSG << ": " << e.what() << std::endl;
    } catch (...) {
        std::cerr << UNKNOWN_EXCEPTION_MSG << std::endl;
    }

    return ERROR;
}
