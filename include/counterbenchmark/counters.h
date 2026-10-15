/**
 * @file counters.h
 * @brief Declares counter implementations with different synchronization strategies.
 */

#ifndef COUNTERBENCHMARK_COUNTERS_H
#define COUNTERBENCHMARK_COUNTERS_H

#include <atomic>
#include <mutex>

#include "counterbenchmark/counter.h"

namespace counterbenchmark {

/**
 * No mutual exclusion: loses updates when used by several threads.
 *
 * Separate atomic loads and stores are individually safe but not atomic
 * together. Relaxed operations model unsynchronized updates without a data
 * race or undefined behavior.
 */
class UnsafeCounter {
public:
    /**
     * Increments the counter using separate relaxed atomic load and store
     * operations.
     *
     * Concurrent calls may overwrite one another, so the final value is not
     * guaranteed to equal the number of calls.
     */
    void increment();

    /**
     * Reads the current counter value.
     *
     * @return The value observed by a relaxed atomic load.
     */
    counter_t get() const;

private:
    /// Atomic storage avoids a data race while allowing lost updates.
    std::atomic<counter_t> value{0};
};

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

/**
 * Uses an atomic read-modify-write operation without an explicit lock.
 *
 * The increment is indivisible with respect to other atomic operations on the
 * same value.
 */
class AtomicCounter {
public:
    /// Atomically increments the counter using the default memory order.
    void increment();

    /**
     * Reads the current counter value.
     *
     * @return The value observed by an atomic load.
     */
    counter_t get() const;

private:
    /// Atomic counter storage.
    std::atomic<counter_t> value{0};
};

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

}  // namespace counterbenchmark

#endif
