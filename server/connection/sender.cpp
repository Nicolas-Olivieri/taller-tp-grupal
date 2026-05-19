#include "sender.h"


Sender::Sender(Socket& peer, Queue<int>& client_queue):
        peer(peer), client_queue(client_queue) {}


void Sender::run() {
    /*
    try {
        while (should_keep_running()) {
            // desencolar evento de la cola del cliente y enviarlo por la red
        }

    } catch (const ClosedQueue&) {
        // Sender finalizado por cola cerrada

    } catch (const ClosedSocket&) {
        if (should_keep_running()) {
            // Sender finalizado por cliente desconectado

        } else {
            // Sender finalizado por servidor desconectado
        }
    }
    */
}
