/**
 * @file spinlock_counter.h
 * @brief Declares a counter protected by a spin lock.
 */

#ifndef SPINLOCK_COUNTER_H
#define SPINLOCK_COUNTER_H

#include <atomic>

#include "counter.h"

/**
 * Protects the counter value with a spin lock.
 *
 * A thread that cannot acquire the lock retries actively rather than
 * suspending.
 */
class SpinLockCounter {
public:
    /// Acquires the spin lock, increments the protected value, and releases it.
    void increment();

    /**
     * Acquires the spin lock and reads the protected value.
     *
     * @return The current counter value.
     */
    counter_t get();

private:
    /// Repeatedly attempts to acquire the spin lock with acquire semantics.
    void lock();

    /// Releases the spin lock with release semantics.
    void unlock();

    /// Atomic flag representing ownership of the spin lock.
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    /// Counter value guarded by @ref flag.
    counter_t value = 0;
};

#endif
