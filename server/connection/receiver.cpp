#include "receiver.h"


Receiver::Receiver(Socket& peer, const std::string& player_name,
                   Queue<std::unique_ptr<Command>>& command_queue):
        protocol(peer), factory(player_name), command_queue(command_queue) {}


void Receiver::run() {
    try {
        while (should_keep_running()) {
            RequestedCommandDTO dto = protocol.recv_command();
            command_queue.push(factory.create(dto));
        }

    } catch (const ClosedSocket&) {
        if (should_keep_running()) {
            // Receiver finalizado por cliente desconectado

        } else {
            // Receiver finalizado por servidor desconectado
        }
    }
}
