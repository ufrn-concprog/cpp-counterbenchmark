/**
 * @file    spinlock_counter.cpp
 * @brief   Implements the spin-lock-protected counter.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include "spinlock_counter.h"

void SpinLockCounter::increment() {
    lock();
    ++value;
    unlock();
}

counter_t SpinLockCounter::get() {
    lock();
    const counter_t result = value;
    unlock();
    return result;
}

void SpinLockCounter::lock() {
    while (flag.test_and_set(std::memory_order_acquire)) {
    }
}

void SpinLockCounter::unlock() {
    flag.clear(std::memory_order_release);
}
