#include "client_sender.h"

#include <memory>
#include <utility>

#include "common/protocol/protocol.h"

ClientSender::ClientSender(Socket& socket): socket(socket) {}

void ClientSender::run() {
    Protocol protocol(socket);

    try {
        while (should_keep_running() && !socket.is_stream_send_closed()) {
            std::unique_ptr<EventDTO> event = sender_q.pop();
            protocol.send(*event);
        }
    } catch (const ClosedQueue& _) {}
}

void ClientSender::close() { sender_q.close(); }

void ClientSender::push(std::unique_ptr<EventDTO>&& event) {
    sender_q.push(std::move(event));
}
