#include "client_handler.h"

#include <utility>


ClientHandler::ClientHandler(Socket&& peer, const std::string& player_name,
                             Queue<std::unique_ptr<Command>>& command_queue,
                             EventBroadcaster& broadcaster):
        peer(std::move(peer)),
        player_name(player_name),
        client_queue(CLIENT_QUEUE_MAX_SIZE),
        sender(this->peer, client_queue, broadcaster),
        receiver(this->peer, player_name, command_queue) {}


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
    return sender.is_alive() or receiver.is_alive();
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
