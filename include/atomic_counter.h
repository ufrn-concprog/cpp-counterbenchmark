/**
 * @file    atomic_counter.h
 * @brief   Declares a counter based on an atomic read-modify-write operation.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#ifndef ATOMIC_COUNTER_H
#define ATOMIC_COUNTER_H

#include <atomic>
using std::atomic;

#include "counter.h"

/**
 * @class AtomicCounter
 * @brief Uses an atomic read-modify-write operation without an explicit lock.
 */
class AtomicCounter {
public:
    /** @brief Atomically increments the counter using the default memory order.*/
    void increment();

    /**
     * @brief Returns the current counter value.
     * @return The value observed by an atomic load.
     */
    counter_t get() const;

private:
    /** @brief Stores the counter's value. */
    atomic<counter_t> value{0};
};

#endif
