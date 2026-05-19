#include "sender.h"

#include "broadcast_register.h"


Sender::Sender(Socket& peer, Queue<SnapshotDTO>& client_queue, EventBroadcaster& broadcaster):
        protocol(peer), client_queue(client_queue), broadcast(broadcaster, this->client_queue) {}


void Sender::run() {
    try {
        while (should_keep_running()) {
            SnapshotDTO snapshot = client_queue.pop();
            protocol.send(snapshot);
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
}
