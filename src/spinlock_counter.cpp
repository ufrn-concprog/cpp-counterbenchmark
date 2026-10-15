/**
 * @file    spinlock_counter.cpp
 * @brief   Implements the spin-lock-protected counter.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include <atomic>
using std::memory_order_acquire;
using std::memory_order_release;

#include "spinlock_counter.h"

/** @brief Acquires the spin lock, increments the protected value, and releases it. */
void SpinLockCounter::increment() {
    lock();
    ++value;
    unlock();
}

/**
 * Acquires the spin lock and reads the protected value.
 * @return The current counter value.
 */
counter_t SpinLockCounter::get() {
    lock();
    const counter_t result = value;
    unlock();
    return result;
}

/** @brief Repeatedly attempts to acquire the spin lock with acquire semantics. */
void SpinLockCounter::lock() {
    while (flag.test_and_set(memory_order_acquire)) {
    }
}

/** @brief Releases the spin lock with release semantics. */
void SpinLockCounter::unlock() {
    flag.clear(memory_order_release);
}
