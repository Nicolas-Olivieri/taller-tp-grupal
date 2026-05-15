#ifndef THREAD_H_
#define THREAD_H_

#include <atomic>
#include <thread>  // NOLINT

class Runnable {
public:
    virtual void start() = 0;
    virtual void join() = 0;
    virtual void stop() = 0;
    virtual bool is_alive() const = 0;

    virtual ~Runnable() {}
};

class Thread: public Runnable {
private:
    std::thread thread;

    // Subclasses that inherit from Thread will have access to these
    // flags, mostly to control how Thread::run() will behave
    std::atomic<bool> _keep_running;
    std::atomic<bool> _is_alive;

protected:
    bool should_keep_running() const;

public:
    Thread();

    void start() override;

    void join() override;

    // Note: it is up to the subclass to make something meaningful to
    // really stop the thread. The Thread::run() may be blocked and/or
    // it may not read _keep_running.
    void stop() override;

    // Note: asking for is_alive is well defined *only if* the thread
    // was started (you called Thread::start())
    bool is_alive() const override;

    void main();

    virtual void run() = 0;
    virtual ~Thread() {}

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

    Thread(Thread&& other) = delete;
    Thread& operator=(Thread&& other) = delete;
};

#endif
