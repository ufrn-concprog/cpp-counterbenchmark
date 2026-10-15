/**
 * @file unsafe_counter.h
 * @brief Declares a counter without mutual exclusion.
 */

#ifndef UNSAFE_COUNTER_H
#define UNSAFE_COUNTER_H

#include <atomic>

#include "counter.h"

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

#endif
