#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

// Thread safe queue to push/pop for runner and charger vehicle

template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;

    void push(const T& v) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(v);
        }
        cv.notify_one();
    }

    // blocking pop()
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]{ return !q.empty(); });
        T t = std::move(q.front());
        q.pop();
        return t;
    }

    // non-blocking pop 
    std::optional<T> tryPop() {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.empty()) return std::nullopt;
        T t = std::move(q.front());
        q.pop();
        return t;
    }

    void notifyAll() { cv.notify_all(); }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }

private:
    mutable std::mutex mtx;
    std::condition_variable cv;
    std::queue<T> q;
};
