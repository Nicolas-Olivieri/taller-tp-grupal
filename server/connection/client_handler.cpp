#include "client_handler.h"

#include <utility>


ClientHandler::ClientHandler(Socket&& peer, const std::string& player_name,
                             Queue<std::unique_ptr<Command>>& command_queue):
        peer_(std::move(peer)),
        player_name(player_name),
        client_queue(CLIENT_QUEUE_MAX_SIZE),
        sender(peer_, client_queue),
        receiver(peer_, command_queue) {}


void ClientHandler::start() {
    sender.start();
    receiver.start();
}


void ClientHandler::stop() { client_queue.close(); }


void ClientHandler::join() {
    sender.join();
    receiver.join();
}


bool ClientHandler::is_alive() const {
    return sender.is_alive() and receiver.is_alive();
}


void ClientHandler::send(const int event) {

    try {
        if (not client_queue.try_push(event)) {
            // Queue llena
        }

    } catch (const ClosedQueue&) {
        // Queue cerrada por desconexión
    }
}


void ClientHandler::polite_kill() {
    sender.stop();
    receiver.stop();
    client_queue.close();
}


void ClientHandler::hard_kill() { polite_kill(); }


const std::string& ClientHandler::get_name() const { return player_name; }


ClientHandler::~ClientHandler() {
    hard_kill();
    join();
}
