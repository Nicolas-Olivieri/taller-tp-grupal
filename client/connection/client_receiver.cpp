#include "client_receiver.h"

#include "common/protocol/protocol.h"


ClientReceiver::ClientReceiver(Socket& socket): socket(socket) {}

void ClientReceiver::run() {

    Protocol protocol(socket);

    try {
        while (should_keep_running() && !socket.is_stream_recv_closed()) {
            SnapshotDTO snapshot = protocol.recv_snapshot();
            receiver_q.push(snapshot);
        }
    } catch (const ClosedSocket& _) {}  // TODO decidir que hacer
}

bool ClientReceiver::try_pop(SnapshotDTO& snapshot) {
//    if (!should_keep_running() || socket.is_stream_recv_closed()) {
//        throw ClosedSocket();
//    }
    return receiver_q.try_pop(snapshot);
}

SnapshotDTO ClientReceiver::pop() { return receiver_q.pop(); }
