/**
 * @file    unsafe_counter.h
 * @brief   Declares a counter without mutual exclusion.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#ifndef UNSAFE_COUNTER_H
#define UNSAFE_COUNTER_H

#include <atomic>
using std::atomic;

#include "counter.h"

/**
 * @class UnsafeCounter
 * @brief A counter that uses separate atomic load and store operations without mutual exclusion.
 */
class UnsafeCounter {
public:
    /**
     * @brief   Increments the counter using separate relaxed atomic load and store operations.
     * @details Concurrent calls may overwrite one another, so the final value is not
     *          guaranteed to equal the number of calls.
     */
    void increment();

    /**
     * Reads the current counter value.
     * @return The value observed by a relaxed atomic load.
     */
    counter_t get() const;

private:
    /** @brief Atomic storage avoids a data race while allowing lost updates. */
    atomic<counter_t> value{0};
};

#endif
