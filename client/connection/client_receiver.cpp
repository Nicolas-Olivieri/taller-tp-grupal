#include "client_receiver.h"

#include "common/protocol/protocol.h"


ClientReceiver::ClientReceiver(Socket& socket): socket(socket) {}

void ClientReceiver::run() {

    Protocol protocol(socket);
    while (should_keep_running() && !socket.is_stream_recv_closed()) {
        SnapshotDTO snapshot = protocol.recv_snapshot();
        receiver_q.push(snapshot);
    }
}

bool ClientReceiver::try_pop(SnapshotDTO& snapshot) {
    return receiver_q.try_pop(snapshot);
}
