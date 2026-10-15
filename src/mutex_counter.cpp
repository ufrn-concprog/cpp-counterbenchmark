/**
 * @file    mutex_counter.cpp
 * @brief   Implements the mutex-protected counter.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#include <mutex>
using std::lock_guard;
using std::mutex;

#include "mutex_counter.h"

/** @brief Locks the mutex and increments the protected value. */
void MutexCounter::increment() {
    lock_guard<mutex> guard(mtx);
    ++value;
}

/**
 * Locks the mutex and reads the protected value.
 * @return The current counter value.
 */
counter_t MutexCounter::get() {
    lock_guard<mutex> guard(mtx);
    return value;
}
