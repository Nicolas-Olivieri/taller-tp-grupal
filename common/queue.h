#ifndef QUEUE_H
#define QUEUE_H

#include <climits>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <utility>

struct ClosedQueue: public std::runtime_error {
    ClosedQueue(): std::runtime_error("The queue is already closed.") {}
};


/*
 * Multi-producer/Multi-consumer Blocking Queue (MP/MC)
 *
 * Queue es una cola MP/MC genérica con operaciones bloqueantes
 * `push()` y `pop()`.
 *
 * Dos métodos adicionales, `try_push()` y `try_pop()` permiten
 * operaciones no bloqueantes.
 *
 * En una cola que ya fue cerrada, cualquiera de los métodos lanzará
 * la excepción `ClosedQueue`.
 * */
template <typename T, class C = std::deque<T> >
class Queue {
private:
    std::queue<T, C> q;
    const unsigned int max_size;

    bool closed;

    std::mutex mtx;
    std::condition_variable is_not_full;
    std::condition_variable is_not_empty;

public:
    Queue(): max_size(UINT_MAX - 1), closed(false) {}

    explicit Queue(const unsigned int max_size): max_size(max_size), closed(false) {}

    bool try_push(T const& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        if (q.size() == max_size) {
            return false;
        }

        if (q.empty()) {
            is_not_empty.notify_all();
        }

        q.push(val);

        return true;
    }

    // Sobrecarga de try_push para pushear por movimiento
    bool try_push(T&& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        if (q.size() == max_size) {
            return false;
        }

        if (q.empty()) {
            is_not_empty.notify_all();
        }

        q.push(std::move(val));

        return true;
    }

    bool try_pop(T& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (q.empty()) {
            if (closed) {
                throw ClosedQueue();
            }

            return false;
        }

        if (q.size() == max_size) {
            is_not_full.notify_all();
        }

        val = std::move(q.front());
        q.pop();

        return true;
    }

    void push(T const& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        while (q.size() == max_size) {
            is_not_full.wait(lock);
        }

        if (q.empty()) {
            is_not_empty.notify_all();
        }

        q.push(val);
    }

    // Sobrecarga de push para pushear por movimiento
    void push(T&& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        while (q.size() == max_size) {
            is_not_full.wait(lock);
        }

        if (q.empty()) {
            is_not_empty.notify_all();
        }

        q.push(std::move(val));
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);

        while (q.empty()) {
            if (closed) {
                throw ClosedQueue();
            }
            is_not_empty.wait(lock);
        }

        if (q.size() == max_size) {
            is_not_full.notify_all();
        }

        T val = std::move(q.front());
        q.pop();

        return val;
    }

    void close() {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        closed = true;
        is_not_empty.notify_all();
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
};


template <>
class Queue<void*> {
private:
    std::queue<void*> q;
    const unsigned int max_size;

    bool closed;

    std::mutex mtx;
    std::condition_variable is_not_full;
    std::condition_variable is_not_empty;

public:
    Queue(): max_size(UINT_MAX - 1), closed(false) {}

    explicit Queue(const unsigned int max_size): max_size(max_size), closed(false) {}

    bool try_push(void* const& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        if (q.size() == max_size) {
            return false;
        }

        if (q.empty()) {
            is_not_empty.notify_all();
        }

        q.push(val);

        return true;
    }

    bool try_pop(void*& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (q.empty()) {
            if (closed) {
                throw ClosedQueue();
            }

            return false;
        }

        if (q.size() == max_size) {
            is_not_full.notify_all();
        }

        val = q.front();
        q.pop();

        return true;
    }

    void push(void* const& val) {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        while (q.size() == max_size) {
            is_not_full.wait(lock);
        }

        if (q.empty()) {
            is_not_empty.notify_all();
        }

        q.push(val);
    }

    void* pop() {
        std::unique_lock<std::mutex> lock(mtx);

        while (q.empty()) {
            if (closed) {
                throw ClosedQueue();
            }

            is_not_empty.wait(lock);
        }

        if (q.size() == max_size) {
            is_not_full.notify_all();
        }

        void* const val = q.front();
        q.pop();

        return val;
    }

    void close() {
        std::unique_lock<std::mutex> lock(mtx);

        if (closed) {
            throw ClosedQueue();
        }

        closed = true;
        is_not_empty.notify_all();
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
};


template <typename T>
class Queue<T*> {
private:
    Queue<void*> q;

public:
    // cppcheck-suppress uninitMemberVar
    Queue() {}

    // cppcheck-suppress uninitMemberVar
    explicit Queue(const unsigned int max_size): q(max_size) {}

    bool try_push(T* const& val) { return q.try_push(val); }

    bool try_pop(T*& val) {
        void* temp = nullptr;
        if (q.try_pop(temp)) {
            val = static_cast<T*>(temp);
            return true;
        }

        return false;
    }

    void push(T* const& val) { return q.push(val); }

    T* pop() { return static_cast<T*>(q.pop()); }

    void close() { return q.close(); }
};


#endif  // QUEUE_H
