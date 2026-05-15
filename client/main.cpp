#include <iostream>

#include "client.h"

#define CLI_MIN_ARGS 3


int main(int argc, char* argv[]) {
    try {
        if (argc != CLI_MIN_ARGS) {
            std::cerr << "" << std::endl;
            return 1;
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
        // TODO agregar log de errores
        std::cerr << "Ocurrió un error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Ocurrió un error Desconocido" << std::endl;
    }
}
