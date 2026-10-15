/**
 * @file    atomic_counter.cpp
 * @brief   Implements the atomic read-modify-write counter.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include "atomic_counter.h"

/** @brief Atomically increments the counter using the default memory order.*/
void AtomicCounter::increment() {
    value.fetch_add(1);
}

/**
 * @brief Returns the current counter value.
 * @return The value observed by an atomic load.
 */
counter_t AtomicCounter::get() const {
    return value.load();
}
