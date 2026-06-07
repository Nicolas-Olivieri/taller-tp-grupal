#include "connection_handler.h"

#include <iostream>
#include <memory>
#include <utility>

#include <sys/socket.h>


ConnectionHandler::ConnectionHandler(Socket&& socket):
        socket(std::move(socket)), sender(this->socket), receiver(this->socket) {}

void ConnectionHandler::start() {
    receiver.start();
    sender.start();
}

void ConnectionHandler::push_command(std::unique_ptr<EventDTO>&& event) { sender.push(std::move(event)); }

void ConnectionHandler::stop() {
    socket.shutdown(SHUT_RDWR);
    socket.close();
    sender.close();

    receiver.join();
    sender.join();
}

ClientMapDataDTO ConnectionHandler::receive_map() { return receiver.receive_map(); }

bool ConnectionHandler::try_pop_snapshot(SnapshotDTO& snapshot) { return receiver.try_pop(snapshot); }

SnapshotDTO ConnectionHandler::pop_snapshot() { return receiver.pop(); }

bool ConnectionHandler::is_finished() { return (!receiver.is_alive()) || (!sender.is_alive()); }
