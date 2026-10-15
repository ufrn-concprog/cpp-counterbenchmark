/**
 * @file mutex_counter.h
 * @brief Declares a counter protected by a mutex.
 */

#ifndef MUTEX_COUNTER_H
#define MUTEX_COUNTER_H

#include <mutex>

#include "counter.h"

/**
 * Protects the counter value with a mutex.
 *
 * A thread contending for the mutex may be suspended by the operating system.
 */
class MutexCounter {
public:
    /// Locks the mutex and increments the protected value.
    void increment();

    /**
     * Locks the mutex and reads the protected value.
     *
     * @return The current counter value.
     */
    counter_t get();

private:
    /// Mutex guarding @ref value.
    std::mutex mtx;
    /// Counter value guarded by @ref mtx.
    counter_t value = 0;
};

#endif
