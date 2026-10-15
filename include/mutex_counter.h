/**
 * @file    mutex_counter.h
 * @brief   Declares a counter protected by a mutex.
 * @author	Everton Cavalcante (everton.cavalcante@ufrn.br)
 * @date	October 14, 2026
 */

#ifndef MUTEX_COUNTER_H
#define MUTEX_COUNTER_H

#include <mutex>
using std::mutex;

#include "counter.h"

/**
 * @class MutexCounter
 * @brief A counter protected by a mutex.
 */
class MutexCounter {
public:
    /** @brief Locks the mutex and increments the protected value. */
    void increment();

    /**
     * Locks the mutex and reads the protected value.
     * @return The current counter value.
     */
    counter_t get();

private:
    /** @brief Mutex guarding @ref value. */
    mutex mtx;
    
    /** @brief Counter value guarded by @ref mtx. */
    counter_t value = 0;
};

#endif
