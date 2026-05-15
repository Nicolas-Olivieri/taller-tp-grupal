#include "thread.h"

#include <iostream>  // TODO: modificar los std::cerr

Thread::Thread(): _keep_running(true), _is_alive(false) {}

bool Thread::should_keep_running() const { return _keep_running; }

void Thread::start() {
    _is_alive = true;
    _keep_running = true;
    thread = std::thread(&Thread::main, this);
}

void Thread::join() { thread.join(); }

void Thread::stop() { _keep_running = false; }

bool Thread::is_alive() const { return _is_alive; }

void Thread::main() {
    try {
        this->run();
    } catch (const std::exception& err) {
        std::cerr << "Unexpected exception: " << err.what() << "\n";
    } catch (...) {
        std::cerr << "Unexpected exception: <unknown>\n";
    }

    _is_alive = false;
}
