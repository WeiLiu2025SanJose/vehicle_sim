#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

/**
 * @brief A thread-safe FIFO queue with blocking and non-blocking pop operations.
 *
 * This class provides a minimal thread-safe queue implementation suitable
 * for producer/consumer patterns used in runner, dispatcher,
 * and charger threads.
 * @tparam T The type of elements stored in the queue.
 */
template<typename T>
class ThreadSafeQueue {
public:
    /**
     * @brief Constructs an empty thread-safe queue.
     */
    ThreadSafeQueue() = default;

    /**
     * @brief Pushes a new element into the queue.
     *
     * This operation is thread-safe and will wake one waiting thread.
     *
     * @param v The value to push into the queue.
     */
    void push(const T& v) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(v);
        }
        cv.notify_one();
    }

    /**
     * @brief Pops an element from the queue, blocking if empty.
     *
     * This function waits until an element is available.
     * It is designed for worker threads that must block during idle time.
     *
     * @return The popped element.
     */
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return !q.empty(); });
        T t = std::move(q.front());
        q.pop();
        return t;
    }

    /**
     * @brief Attempts to pop an element without blocking.
     *
     * @return std::optional<T> containing the element if available,
     *         or std::nullopt if the queue is empty.
     */
    std::optional<T> tryPop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.empty()) return std::nullopt;
        T t = std::move(q.front());
        q.pop();
        return t;
    }

    /**
     * @brief Wakes all threads waiting on pop().
     *
     * Used during shutdown to release blocked threads.
     */
    void notifyAll() { cv.notify_all(); }

    /**
     * @brief Checks whether the queue is empty.
     *
     * This function is thread-safe.
     *
     * @return true if the queue is empty, false otherwise.
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

    /**
     * @brief Returns the number of elements in the queue.
     *
     * @return Number of elements currently stored.
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }

private:
    mutable std::mutex mtx;              ///< Protects access to the queue
    std::condition_variable cv;          ///< Used to block/wake waiting threads
    std::queue<T> q;                     ///< The underlying standard queue
};
