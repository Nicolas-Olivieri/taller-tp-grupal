#include "game_loop.h"

#include "common/util/rate_timer.h"

#define FPS 30


GameLoop::GameLoop(Queue<std::unique_ptr<Command>>& command_queue):
        command_queue(command_queue), game_world(10, 10) {}


void GameLoop::run() {
    RateTimer timer(FPS);
    int iteration = 0;

    while (should_keep_running()) {
        process_commands();
        update_world(iteration);
        iteration = timer.calculate_next_iteration();
    }
}


void GameLoop::process_commands() {
    try {
        std::unique_ptr<Command> cmd;
        while (command_queue.try_pop(cmd)) {
            cmd->execute(game_world);
            // broadcast(...);
        }
    } catch (const ClosedQueue&) {}
}


void GameLoop::update_world(const int /* iteration */) {
    // TODO: Implementar lógica
}


// void GameLoop::broadcast(...) const {
//     // TODO: Implementar lógica
// }


GameLoop::~GameLoop() {
    Thread::stop();
    Thread::join();
}
