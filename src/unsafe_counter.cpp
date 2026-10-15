/**
 * @file    unsafe_counter.cpp
 * @brief   Implements the counter without mutual exclusion.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include <atomic>
using std::memory_order_relaxed;

#include "unsafe_counter.h"

/**
 * @brief   Increments the counter using separate relaxed atomic load and store operations.
 * @details Concurrent calls may overwrite one another, so the final value is not
 *          guaranteed to equal the number of calls.
 */
void UnsafeCounter::increment() {
    value.store(value.load(memory_order_relaxed) + 1, memory_order_relaxed);
}

/**
 * Reads the current counter value.
 * @return The value observed by a relaxed atomic load.
 */
counter_t UnsafeCounter::get() const {
    return value.load(memory_order_relaxed);
}
