#pragma once
// Adapted from https://stackoverflow.com/a/46381610
namespace cs474 {
namespace utils {
class atomic_mutex {
public:
    void lock() {
        while (flag.exchange(true, std::memory_order_relaxed));
        std::atomic_thread_fence(std::memory_order_acquire);
    }

    void unlock() {
        std::atomic_thread_fence(std::memory_order_release);
        flag.store(false, std::memory_order_relaxed);
    }

private:
    std::atomic<bool> flag{ false };
};

class atomic_guard {
public:
    atomic_guard(atomic_mutex& mutex) : m_Mutex(mutex) { m_Mutex.lock(); }
    ~atomic_guard() { m_Mutex.unlock(); }

private:
    atomic_mutex& m_Mutex;
};
}
}