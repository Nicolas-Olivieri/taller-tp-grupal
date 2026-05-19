#include "receiver.h"


Receiver::Receiver(Socket& peer,
                   Queue<std::unique_ptr<Command>>& command_queue):
        peer(peer), command_queue(command_queue) {}


void Receiver::run() {
    /*
    try {
        while (should_keep_running()) {
            // recibir un comando por la red y encolarlo en la cola de comandos
        }

    } catch (const ClosedSocket&) {
        if (should_keep_running()) {
            // Receiver finalizado por cliente desconectado

        } else {
            // Receiver finalizado por servidor desconectado
        }
    }
    */
}
