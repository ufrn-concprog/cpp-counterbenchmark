/**
 * @file atomic_counter.h
 * @brief Declares a counter based on an atomic read-modify-write operation.
 */

#ifndef ATOMIC_COUNTER_H
#define ATOMIC_COUNTER_H

#include <atomic>

#include "counter.h"

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

#endif
