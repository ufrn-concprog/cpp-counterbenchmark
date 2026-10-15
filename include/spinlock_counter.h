/**
 * @file    spinlock_counter.h
 * @brief   Declares a counter protected by a spin lock.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#ifndef SPINLOCK_COUNTER_H
#define SPINLOCK_COUNTER_H

#include <atomic>
using std::atomic_flag;

#include "counter.h"

/**
 * @class SpinLockCounter
 * @brief A counter protected by a spin lock.
 */
class SpinLockCounter {
public:
    /** @brief Acquires the spin lock, increments the protected value, and releases it. */
    void increment();

    /**
     * Acquires the spin lock and reads the protected value.
     * @return The current counter value.
     */
    counter_t get();

private:
    /** @brief Repeatedly attempts to acquire the spin lock with acquire semantics. */
    void lock();

    /** @brief Releases the spin lock with release semantics. */
    void unlock();

    /** @brief Atomic flag representing ownership of the spin lock. */
    atomic_flag flag = ATOMIC_FLAG_INIT;
    
    /** @brief Counter value guarded by @ref flag. */
    counter_t value = 0;
};

#endif
