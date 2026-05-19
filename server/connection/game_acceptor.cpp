#include "game_acceptor.h"

#include <utility>

#include "server/command/despawn_command.h"
#include "server/command/spawn_command.h"


GameAcceptor::GameAcceptor(Queue<ConnectionInfo>& waiting_queue):
        waiting_queue(waiting_queue), game_loop(command_queue) {}


void GameAcceptor::run() {
    game_loop.start();
    try {
        while (should_keep_running()) {
            auto [player_name, peer] = waiting_queue.pop();
            reap();
            clients.emplace_back(std::move(peer), player_name, command_queue,
                                 broadcaster);
            clients.back().start();
            command_queue.push(std::make_unique<SpawnCommand>(player_name));
        }
    } catch (const ClosedQueue&) {}
}


void GameAcceptor::reap() {
    auto it = clients.begin();
    while (it != clients.end()) {
        if (not it->is_alive()) {
            command_queue.push(
                    std::make_unique<DespawnCommand>(it->get_name()));
            it = clients.erase(it);
        } else {
            ++it;
        }
    }
}


GameAcceptor::~GameAcceptor() {
    command_queue.close();
    Thread::stop();
    Thread::join();
}
