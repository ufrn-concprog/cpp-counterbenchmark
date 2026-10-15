/**
 * @file counters.cpp
 * @brief Implements the counter synchronization strategies.
 */

#include "counterbenchmark/counters.h"

namespace counterbenchmark {

void UnsafeCounter::increment() {
    value.store(value.load(std::memory_order_relaxed) + 1,
                std::memory_order_relaxed);
}

counter_t UnsafeCounter::get() const {
    return value.load(std::memory_order_relaxed);
}

void MutexCounter::increment() {
    std::lock_guard<std::mutex> guard(mtx);
    ++value;
}

counter_t MutexCounter::get() {
    std::lock_guard<std::mutex> guard(mtx);
    return value;
}

void AtomicCounter::increment() {
    value.fetch_add(1);
}

counter_t AtomicCounter::get() const {
    return value.load();
}

void SpinLockCounter::increment() {
    lock();
    ++value;
    unlock();
}

counter_t SpinLockCounter::get() {
    lock();
    const counter_t result = value;
    unlock();
    return result;
}

void SpinLockCounter::lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
    }
}

void SpinLockCounter::unlock() {
    flag.clear(std::memory_order_release);
}

}  // namespace counterbenchmark
