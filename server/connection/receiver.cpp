#include "receiver.h"

#include "server/command/despawn_command.h"


Receiver::Receiver(Socket& peer, const std::string& player_name,
                   Queue<std::unique_ptr<Command>>& command_queue):
        protocol(peer), player_name(player_name), factory(player_name), command_queue(command_queue) {}


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

    command_queue.push(std::make_unique<DespawnCommand>(player_name));
}
