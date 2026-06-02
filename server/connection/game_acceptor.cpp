#include "game_acceptor.h"

#include <utility>

#include "server/command/cmd_types/cmd_spawn/spawn_command.h"


GameAcceptor::GameAcceptor(Queue<ConnectionInfo>& waiting_queue, PlayerRepository& player_repository):
        waiting_queue(waiting_queue),
        player_repository(player_repository),
        game_loop(command_queue, broadcaster, player_repository) {}


void GameAcceptor::run() {
    game_loop.start();
    try {
        while (should_keep_running()) {
            auto [player_name, peer] = waiting_queue.pop();
            reap();
            clients.emplace_back(std::move(peer), player_name, command_queue, broadcaster);
            clients.back().start();

            PlayerData data = player_repository.get(player_name);
            command_queue.push(std::make_unique<SpawnCommand>(player_name, data));
        }
    } catch (const ClosedQueue&) {}
}


void GameAcceptor::reap() {
    auto it = clients.begin();
    while (it != clients.end()) {
        if (not it->is_alive()) {
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
